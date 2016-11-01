package sic.macros;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.management.RuntimeErrorException;

public class IfWhileHandler {
	private HashMap<String, Integer> setMap = new HashMap<String, Integer>();
	private ArrayList<MacrosCommand> expandedCommands = new ArrayList<MacrosCommand>();
	private ArrayList<ArgumentExtractor> argument;

	public ArrayList<ArgumentExtractor> getArgument() {
		return argument;
	}

	public void setArgument(ArrayList<ArgumentExtractor> argument) {
		this.argument = argument;
	}

	public ArrayList<MacrosCommand> expandIf(ArrayList<MacrosCommand> ifScope) {

		String condition = ifScope.get(0).getOperand();
		boolean ifConditionSatisfied = checkCondition(condition);
		boolean elseScope = false;
		for (int i = 1; i < ifScope.size(); i++) {
			if (ifConditionSatisfied) {
				if (ifScope.get(i).getMnemonic().equalsIgnoreCase("ELSE")
						|| ifScope.get(i).getMnemonic().equalsIgnoreCase("ENDIF")) {
					break;
				}
				processLine(ifScope.get(i));
			} else if (!ifConditionSatisfied && !ifScope.get(i).getMnemonic().equalsIgnoreCase("ENDIF")) {
				if (!elseScope && ifScope.get(i).getMnemonic().equalsIgnoreCase("ELSE")) {
					elseScope = true;
				} else if (elseScope) {
					processLine(ifScope.get(i));
				}

			}

		}
		return expandedCommands;

	}

	public ArrayList<MacrosCommand> expandWhile(ArrayList<MacrosCommand> whileScope) {
		String condition = whileScope.get(0).getOperand();
		MacrosCommand command;
		while (checkCondition(condition)) {
			int index = 1;
			do {
				command = whileScope.get(index++);
				if (!command.getMnemonic().equalsIgnoreCase("ENDW")) {
					processLine(command);
				}
			} while (!command.getMnemonic().equalsIgnoreCase("ENDW") && index < whileScope.size());

		}
		return expandedCommands;

	}

	public void processLine(MacrosCommand command) { // changed
		SetParser parser = new SetParser();
		parser.setSetMap(setMap);
		if (command.getMnemonic().equalsIgnoreCase("SET")) {
			parser.parse(command);
			setMap = parser.getSetMap();
		} else {
			MacrosCommand temp = parser.substiuteArgument(command, argument);
			setMap = parser.getSetMap();
			expandedCommands.add(temp);
		}
	}

	public boolean checkCondition(String condition) {

		String pattern = "(\\&\\w+|\\d+)\\s+(GT|LT|EQ|LE|GE)\\s+(\\&\\w+|\\d+)";
		float rightSide = 0;
		float leftSide = 0;

		// Create a Pattern object
		Pattern r = Pattern.compile(pattern);

		// Now create matcher object.
		Matcher m = r.matcher(condition);
		m.find();
		if (m.group(1).matches("-?\\d+(\\.\\d+)?")) {
			leftSide = Float.parseFloat(m.group(1));
		} else if (setMap.containsKey(m.group(1))) {
			leftSide = setMap.get(m.group(1));

		} else if (m.group(1).matches("\\&\\w+")) {
			leftSide = 0;
			setMap.put(m.group(1), 0);
		} else {
			throw new RuntimeErrorException(null, "Unexpected operand");
		}

		if (m.group(3).matches("-?\\d+(\\.\\d+)?")) {
			rightSide = Float.parseFloat(m.group(3));
		} else if (setMap.containsKey(m.group(3))) {
			rightSide = setMap.get(m.group(3));
		} else if (m.group(3).matches("\\&\\w+")) {
			rightSide = 0;
			setMap.put(m.group(3), 0);
		} else {
			throw new RuntimeErrorException(null, "Unexpected operand");
		}
		if (m.group(2).equalsIgnoreCase("EQ")) {
			if (leftSide == rightSide) {
				return true;
			}
		} else if (m.group(2).equalsIgnoreCase("LT")) {
			if (leftSide < rightSide) {
				return true;
			}

		} else if (m.group(2).equalsIgnoreCase("GT")) {
			if (leftSide > rightSide) {
				return true;
			}

		} else if (m.group(2).equalsIgnoreCase("LE")) {
			if (leftSide <= rightSide) {
				return true;
			}

		} else if (m.group(2).equalsIgnoreCase("GE")) {
			if (leftSide >= rightSide) {
				return true;
			}

		} else {

			throw new RuntimeErrorException(null, "Unexpected condition");
		}

		return false;

	}

	public HashMap<String, Integer> getSetMap() {
		return setMap;
	}

	public void setSetMap(HashMap<String, Integer> setMap) {
		this.setMap = setMap;
	}

}
