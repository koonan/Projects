package sic.macros;

import sic.asm.Location;

public class MacrosCommand {

	private Location loc = null;
	private String label = null;
	private String mnemonic = null;
	private String operand = null;
	private boolean iscomment = false;
	protected String comment;

	public MacrosCommand(Location loc, String label, String mnemonic, String operand) {

		if (loc != null) {
			this.loc = loc;
		}
		if (label != null) {
			this.label = label;
		}
		if (operand != null) {
			this.operand = operand.trim();
		}
		if (mnemonic != null) {
			this.mnemonic = mnemonic;
		}
	}

	public MacrosCommand(String comment) {

		this.setComment(comment);
		iscomment = true;
	}

	public MacrosCommand clone() {
		MacrosCommand newCommand = new MacrosCommand(this.loc, this.label(), this.getMnemonic(), this.getOperand());
		newCommand.iscomment = this.iscomment;
		newCommand.setComment(this.getComment());

		return newCommand;

	}

	// getters/setters

	public boolean isComment() {
		return iscomment;
	}

	public boolean hasLabel() {
		return label != null;
	}

	public boolean hasOperand() {
		return operand != null;
	}

	public String label() {
		return hasLabel() ? label : "";
	}

	public void setLabel(String label) {
		this.label = label;
	}

	public String getMnemonic() {
		return hasMnemonic() ? mnemonic : "";
	}

	public boolean hasMnemonic() {
		return mnemonic != null;
	}

	public boolean hasComment() {
		return comment != null;
	}

	public String getComment() {
		return hasComment() ? comment : "";
	}

	public String getOperand() {
		return hasOperand() ? operand : "";
	}

	public void setOperand(String operand) {
		this.operand = operand;
	}

	public void setComment(String comment) {
		if (comment != null)
			comment = comment.trim();
		this.comment = comment;
	}

	public String comment() {
		if (comment == null || comment.length() == 0)
			return "";
		return (comment.startsWith(".") ? "." : ". ") + comment;
	}

}
