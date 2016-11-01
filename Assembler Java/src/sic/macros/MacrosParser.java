package sic.macros;

import sic.asm.AsmError;

import sic.asm.Location;
import sic.asm.Options;

public class MacrosParser extends MacrosLexer {

	public void checkWhitespace(String fmt, Object... objs) throws AsmError {
		if (Options.requireWhitespace && !Character.isWhitespace(prev))
			throw new AsmError(loc(), fmt, objs);
	}

	public MacrosCommand parseIfCommand() throws AsmError {
		Location loc = loc();
		String label = readIfLabel();
		skipWhitespace();
		if (label != null)
			checkWhitespace("Missing whitespace after label '%s'", label);
		else
			loc = loc();
		String mnemonic = readIfMnemonic();

		if (mnemonic == null) {
			if (label == null)
				return null; // no instruction present
			throw new AsmError(loc(), "Missing mnemonic");
		}

		skipWhitespace();
		// read operand
		String operand = readIfOperand();

		return new MacrosCommand(loc, label, mnemonic, operand);
	}

	public MacrosCommand parseProgram() {
		MacrosCommand command = null;

		if (ready()) {
			assert col() == 1;

			String comment = null;
			try {
				command = parseIfCommand();
				skipWhitespace();
				comment = readIfComment(Options.requireCommentDot, Options.skipEmptyLines);
				if (command == null && comment == null) {
					advance('\n'); // advance over the empty line
				}

			} catch (AsmError e) {
				advanceUntil('\n');
			}

			if (command != null) {
				// command with possible comment
				command.setComment(comment);

			} else if (comment != null) {
				// only comment
				command = new MacrosCommand(comment);

			}
		}

		return command;
	}

}
