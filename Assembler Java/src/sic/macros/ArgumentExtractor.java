package sic.macros;

import java.util.ArrayList;

public class ArgumentExtractor {
	
	private ArrayList<String> argumentList;
	private String argument;
	private boolean isList;
	
	public ArgumentExtractor() {
		this.argumentList = new ArrayList<String>();
		this.isList = false;
	}

	public String getArgumentList(int index) {
		return argumentList.get(index);
	}

	public void setArgumentList(ArrayList<String> argument) {
		this.argumentList = argument;
	}

	public boolean isList() {
		return isList;
	}

	public void setList(boolean isList) {
		this.isList = isList;
	}

	public String getArgument() {
		return argument;
	}

	public void setArgument(String argument) {
		this.argument = argument;
	}
	public int getListLenght(){
		
		return this.argumentList.size();
	}
	
	

}
