package sic.macros;

import java.io.FileNotFoundException;

import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;

public class ExpandedFileWriter {

	private PrintWriter writer;
	private String program = "";

	public String getProgram() {
		return program;
	}

	public ExpandedFileWriter() {
		try {
			writer = new PrintWriter("expanded.txt", "UTF-8");
		} catch (FileNotFoundException e) {

			e.printStackTrace();
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}

	}

	public void write(MacrosCommand command) {

		String formatted;
		if (command.isComment()) {
			formatted = "." + command.getComment();
		} else {
			formatted = String.format("%-9s%-8s%-18s%-31s", command.label(), command.getMnemonic(),
					command.getOperand(), command.getComment());
		}
		formatted = formatted.toUpperCase();
		program = program + "\n" + formatted;
		writer.println(formatted);
	}

	public void close() {
		writer.close();

	}
}
