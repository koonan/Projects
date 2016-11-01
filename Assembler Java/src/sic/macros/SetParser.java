package sic.macros;

import java.util.ArrayList;

import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.management.RuntimeErrorException;

public class SetParser {
	private HashMap<String, Integer> setMap = new HashMap<String, Integer>();

	public HashMap<String, Integer> getSetMap() {
		return setMap;
	}

	public void setSetMap(HashMap<String, Integer> setMap) {
		this.setMap = setMap;
	}

	public boolean isInteger(String number) {
		try {
			Integer.parseInt(number);
			return true;
		} catch (NumberFormatException e) {
			return false;
		}

	}

	public void parse(MacrosCommand command) {
		String operand = command.getOperand();
		String splittedOperand[] = operand.split("[+|-|*|/]");
		int rightSide = 0;
		int leftSide = 0;

		splittedOperand[0] = splittedOperand[0].trim();
		if (isInteger(splittedOperand[0])) {
			leftSide = Integer.parseInt(splittedOperand[0]);
		} else if (setMap.containsKey(splittedOperand[0])) {
			leftSide = setMap.get(splittedOperand[0]);
		} else if (splittedOperand[0].matches("\\&\\w+")) {
			leftSide = 0;
			setMap.put(splittedOperand[0], 0);
		} else {

			throw new RuntimeErrorException(null, "Unexpected operand");
		}

		if (splittedOperand.length != 1) {
			splittedOperand[1] = splittedOperand[1].trim();

			if (isInteger(splittedOperand[1])) {
				rightSide = Integer.parseInt(splittedOperand[1]);
			} else if (setMap.containsKey(splittedOperand[1])) {
				rightSide = setMap.get(splittedOperand[1]);
			} else if (splittedOperand[1].matches("\\&\\w+")) {
				rightSide = 0;
				setMap.put(splittedOperand[1], 0);
			} else {
				throw new RuntimeErrorException(null, "Unexpected operand");
			}

			if (operand.contains("+")) {
				setMap.put(command.label(), (rightSide + leftSide));

			} else if (operand.contains("-")) {

				setMap.put(command.label(), (rightSide - leftSide));

			} else if (operand.contains("*")) {
				setMap.put(command.label(), (rightSide * leftSide));

			} else if (operand.contains("/")) {
				setMap.put(command.label(), (rightSide / leftSide));

			} else if (operand.contains("%")) {
				setMap.put(command.label(), (rightSide % leftSide));

			} else {

				throw new RuntimeErrorException(null, "Unexpected condition");
			}
		} else {
			setMap.put(command.label(), leftSide);
		}

	}

	public MacrosCommand substiuteArgument(MacrosCommand command, ArrayList<ArgumentExtractor> argument) {
		String pattern = "(\\?(\\d+)(\\((\\d+|\\&\\w+)\\))?|(\\&\\w+))";
		Pattern r = Pattern.compile(pattern);
		String operand = command.getOperand();
		Matcher m = r.matcher(operand);
		while (m.find()) {
			// check for time Variable as operand
			if (m.group(2) == null) {
				int timeVariableValue = 0;
				if (setMap.containsKey(m.group(1))) {
					timeVariableValue = setMap.get(m.group(1));
				} else if (m.group(1).matches("\\&\\w+")) {
					timeVariableValue = 0;
					setMap.put(m.group(1), 0);
				} else {
					throw new RuntimeErrorException(null, "Unexpected operand");
				}
				operand = operand.replace(m.group(1), String.valueOf(timeVariableValue));
				// index of list is a time variable
			} else {

				int num = Integer.parseInt(m.group(2)) - 1;
				if (num >= 0 && num < argument.size()) {
					ArgumentExtractor arg = argument.get(num);

					int numList = 0;
					if (setMap.containsKey(m.group(4))) {
						numList = setMap.get(m.group(4)) - 1;
					} else if (m.group(4).matches("\\&\\w+")) {
						numList = 0;
						setMap.put(m.group(4), 0);
					} else {
						throw new RuntimeErrorException(null, "Unexpected operand");
					}

					String parameter = arg.getArgumentList(numList);
					operand = operand.replace(m.group(1), parameter);
				}

			}

		}
		return new MacrosCommand(null, command.label(), command.getMnemonic(), operand);

	}

}
