package cycle;

import java.util.Vector;

public class AdjacencyList {
	
	public static int[][] getAdjacencyList(boolean[][] adjacencyMatrix) {
		int[][] list = new int[adjacencyMatrix.length][];

		for (int i = 0; i < adjacencyMatrix.length; i++) {
			Vector<Integer> v = new Vector<Integer>();
			for (int j = 0; j < adjacencyMatrix[i].length; j++) {
				if (adjacencyMatrix[i][j]) {
					v.add(new Integer(j));
				}
			}

			list[i] = new int[v.size()];
			for (int j = 0; j < v.size(); j++) {
				Integer in = (Integer) v.get(j);
				list[i][j] = in.intValue();
			}
		}
		
		return list;
	}
}
