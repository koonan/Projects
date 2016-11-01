package sic.macros;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class OperandHandler {
	
   // Function to replace the arguments with positional notation 
	public String substitute(ArrayList<String> arguments, MacrosCommand macrosCommand) {

		String line = macrosCommand.getOperand();

		for (String token : line.split("[^\\&\\w]")) {
			if (token.contains("&")) {
				int index = arguments.indexOf(token) + 1;

				if (index > 0) {
					line = line.replace(token, ("?" + index));
				}
			}
		}

		return line;
	}

	//extracts arguments
	public ArrayList<ArgumentExtractor> extractArgument(String line) {

		ArrayList<ArgumentExtractor> argumentList = new ArrayList<ArgumentExtractor>();
		line = line.trim();
		String pattern = "(\\((.*?)\\))";
		Pattern r = Pattern.compile(pattern);
		Matcher m = r.matcher(line);
		
		while (m.find()) {
			String oldString = m.group(1);
			String newString = m.group(0).replace(" ", "").replace(',', ' ');
			line = line.replace(oldString, newString);
		}

		String[] parameters = line.split(",");
		for (int i = 0; i < parameters.length && parameters.length > 1; i++) {

			ArgumentExtractor argument = new ArgumentExtractor();
			parameters[i] = parameters[i].trim();
			if (parameters[i].charAt(0) == '(') {
				parameters[i] = (String) parameters[i].subSequence(1, parameters[i].length() - 1);
				parameters[i] = parameters[i].trim();
				ArrayList<String> arr = new ArrayList<String>(Arrays.asList(parameters[i].split("\\s+")));
				argument.setArgumentList(arr);
				argument.setList(true);

			} else {
				argument.setArgument(parameters[i]);
				argument.setList(false);

			}
			argumentList.add(argument);
		}

		return argumentList;
	}

	public String substiuteArgument(String operand, ArrayList<ArgumentExtractor> argument) {
		String pattern = "(\\?(\\d+)(\\((\\d+|\\&\\w+)\\))?)";
		Pattern r = Pattern.compile(pattern);
		Matcher m = r.matcher(operand);
		while (m.find()) {
			int num = Integer.parseInt(m.group(2)) - 1;
			if (num >= 0 && num < argument.size()) {
				ArgumentExtractor arg = argument.get(num);

				if (m.group(1).matches("(\\?(\\d+)(\\((\\&\\w+)\\)))")) {
					continue;
				} else if (arg.isList()) {
					int numList = Integer.parseInt(m.group(4)) - 1;
					String parameter = arg.getArgumentList(numList);
					operand = operand.replace(m.group(1), parameter);
				} else {
					String parameter = arg.getArgument();

					operand = operand.replace(m.group(1), parameter);
				}
			} else {
				throw new RuntimeException("The Number is outOfBounds ");
			}

		}

		return operand;

	}

	public String substituteItemsLenght(String operand, ArrayList<ArgumentExtractor> argument) {
		String pattern = "(\\%ITEMS\\(\\?(\\d+)\\))";
		Pattern r = Pattern.compile(pattern);
		Matcher m = r.matcher(operand);
		while (m.find()) {
			int num = Integer.parseInt(m.group(2)) - 1;
			if (num >= 0 && num < argument.size()) {
				ArgumentExtractor arg = argument.get(num);
				if (arg.isList()) {
					String numList = Integer.toString(arg.getListLenght());
					operand = operand.replace(m.group(1), numList);
				} else {
					throw new RuntimeException("The parameter is not list");
				}
			} else {
				throw new RuntimeException("The Number is outOfBounds ");
			}

		}

		return operand;
	}
}
