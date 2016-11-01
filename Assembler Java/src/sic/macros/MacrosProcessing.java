package sic.macros;

import java.awt.Point;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

import sic.asm.AsmError;

public class MacrosProcessing {
	private MacrosCommand macrosCommand;
	private MacrosParser parser;
	private String marcoName;
	private LabelHandler labelParser;
	private ExpandedFileWriter writer;
	private Map<String, ArrayList<MacrosCommand>> defTable;
	private OperandHandler operandParser;
	private ArrayList<ArgumentExtractor> argumentList;
	private HashMap<String, Integer> setMap;
	private HashMap<String, Point> labelNum;
	private String labelNumCombination;
	private SetParser set;
	private Stack<ArrayList<ArgumentExtractor>> argStack;

	public MacrosProcessing(MacrosParser parser) {
		this.argStack = new Stack<ArrayList<ArgumentExtractor>>();
		this.parser = parser;
		this.writer = new ExpandedFileWriter();
		this.setMap = new HashMap<String, Integer>();
		this.operandParser = new OperandHandler();
		this.defTable = new HashMap<String, ArrayList<MacrosCommand>>();
		this.labelNum = new HashMap<>();
		this.set = new SetParser();
		this.labelParser = new LabelHandler();

	}

	public String begin() throws AsmError {
		do {
			macrosCommand = getLine();
			if (macrosCommand == null) {
				throw new AsmError(parser.loc(), "Missing End");
			}
			processLine();
		} while (!macrosCommand.getMnemonic().equalsIgnoreCase("END"));
		String result = writer.getProgram();
		writer.close();
		return result;
	}

	private void processLine() throws AsmError {

		marcoName = macrosCommand.getMnemonic();
		if (defTable.containsKey(marcoName)) {
			this.expand();
		} else if (marcoName.equalsIgnoreCase("MACRO")) {
			define();
		} else {

			writer.write(macrosCommand);
		}

	}

	private MacrosCommand getLine() {

		return parser.parseProgram();
	}

	private void define() throws AsmError {
		int level = 1;
		ArrayList<MacrosCommand> commands = new ArrayList<MacrosCommand>();
		String macroName = macrosCommand.label();
		Point point = new Point();
		point.x = 0;
		point.y = 0;
		labelNum.put(macroName, point);
		commands.add(macrosCommand);
		// split on ,
		ArrayList<String> arguments = new ArrayList<String>(
				Arrays.asList(macrosCommand.getOperand().trim().split(",")));

		// HANDLE FOR (MEND)
		while (level > 0) {

			macrosCommand = getLine();
			if (macrosCommand == null) {

				throw new AsmError(parser.loc(), "Missing MEND");
			}
			if (!macrosCommand.isComment()) {

				if (macrosCommand.getMnemonic().equalsIgnoreCase("MACROS")) {

					level++;

				} else if (macrosCommand.getMnemonic().equalsIgnoreCase("MEND")) {

					level--;
				} else {
					macrosCommand.setOperand(operandParser.substitute(arguments, macrosCommand));
					commands.add(macrosCommand);

				}
			}
		}

		defTable.put(macroName, commands);
	}

	private void expand() throws AsmError {

		ArrayList<MacrosCommand> macros = new ArrayList<MacrosCommand>();

		for (int i = 0; i < defTable.get(marcoName).size(); i++) {
			MacrosCommand command = defTable.get(marcoName).get(i);
			macros.add(command.clone());
		}
		// store arguments for successive macro calls
		argumentList = new ArrayList<>();
		argStack.push(operandParser.extractArgument(macrosCommand.getOperand()));
		argumentList = operandParser.extractArgument(macrosCommand.getOperand());

		Point point = labelNum.get(marcoName);
		if (point.x == 26 && point.y == 26) {
			throw new AsmError(parser.loc(), "The limite of expand exceed");
		} else {
			labelNumCombination = "" + (char) (point.x + 65) + (char) (point.y + 65);
			if (point.y == 25) {
				point.x++;
				point.y = 0;
			} else {

				point.y++;
			}
		}

		for (int i = 1; i < macros.size(); i++) {
			this.macrosCommand = macros.get(i);

			if (this.macrosCommand.getMnemonic().equalsIgnoreCase("IF")) {
				i = this.handleIfCondition(macros, i);

			} else if (this.macrosCommand.getMnemonic().equalsIgnoreCase("WHILE")) {
				i = this.handleWhilleCondition(macros, i);

			} else if (this.macrosCommand.getMnemonic().equalsIgnoreCase("SET")) {// changed

				this.substitute();
				set.setSetMap(setMap);
				try{
					set.parse(macrosCommand);
				}catch(RuntimeException e){
					throw new AsmError(parser.loc(), "Unexpected operand");
				}
				
				setMap = set.getSetMap();
			} else {// changed

				this.substitute();
				this.processLine();

			}

		}

		argStack.pop();
		if (!argStack.isEmpty()) {
			argumentList = argStack.peek();
		}
	}

	private void substitute() throws AsmError {
		String operand = this.macrosCommand.getOperand();
		String label = this.macrosCommand.label();
		String newOperand1 = null;

		operand = operand.toUpperCase();
		try{
			if (operand.contains("%ITEMS")) {
				operand = operandParser.substituteItemsLenght(operand.trim(), argumentList); // changed
			}
			newOperand1 = operandParser.substiuteArgument(operand, argumentList);
		}catch(RuntimeException e){
			
			throw new AsmError(parser.loc(), "The Number is wrong");
		}
		
		String newOperand2 = labelParser.substituteLabel(newOperand1, labelNumCombination);

		this.macrosCommand.setOperand(newOperand2);
		if (!label.equalsIgnoreCase("")) {
			String newLabel = labelParser.substituteLabel(label, labelNumCombination);
			this.macrosCommand.setLabel(newLabel);
		}

	}

	private int handleIfCondition(ArrayList<MacrosCommand> macros, int advance) throws AsmError {

		IfWhileHandler expandCondition = new IfWhileHandler();
		expandCondition.setArgument(argumentList);
		ArrayList<MacrosCommand> ifBody = new ArrayList<MacrosCommand>();
		while (!this.macrosCommand.getMnemonic().equalsIgnoreCase("ENDIF")) {

			this.substitute();
			ifBody.add(this.macrosCommand);
			if (advance >= macros.size() - 1) {
				throw new AsmError(parser.loc(), "Missing ENDIF");

			}
			this.macrosCommand = macros.get(++advance);

		}
		try{
			expandCondition.setSetMap(setMap);
			ArrayList<MacrosCommand> afterExpand = expandCondition.expandIf(ifBody);
			for (int j = 0; j < afterExpand.size(); j++) {
				this.macrosCommand = afterExpand.get(j);
				processLine();
			}
		}catch(RuntimeException e){
			throw new AsmError(parser.loc(), "Unexpected operand");
		}
		
		return advance;
	}

	private int handleWhilleCondition(ArrayList<MacrosCommand> macros, int advance) throws AsmError {

		IfWhileHandler expandCondition = new IfWhileHandler();
		expandCondition.setArgument(argumentList);
		ArrayList<MacrosCommand> whileBody = new ArrayList<MacrosCommand>();
		while (!this.macrosCommand.getMnemonic().equalsIgnoreCase("ENDW")) {
			this.substitute();
			whileBody.add(this.macrosCommand);
			if (advance >= macros.size() - 1) {
				throw new AsmError(parser.loc(), "Missing ENDWHILE");

			}
			this.macrosCommand = macros.get(++advance);
		}
		try{
			expandCondition.setSetMap(setMap);
			ArrayList<MacrosCommand> afterExpand = expandCondition.expandWhile(whileBody);
			for (int j = 0; j < afterExpand.size(); j++) {
				this.macrosCommand = afterExpand.get(j);
				processLine();
			}
		}catch(RuntimeException e){
			throw new AsmError(parser.loc(), "Unexpected operand");
		}
		
		return advance;
	}

}
