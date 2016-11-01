package sic.macros;

public class LabelHandler {
	// replace labels with the expansion counter
	public String substituteLabel(String symbol, String numOfExpanding) {

		for (String token : symbol.split("[^\\$\\w]")) {
			if (token.contains("$")) {
				String subToken = token.replace("$", "");
				symbol = symbol.replace(token, "$" + numOfExpanding + subToken);

			}
		}
		return symbol;

	}
}
