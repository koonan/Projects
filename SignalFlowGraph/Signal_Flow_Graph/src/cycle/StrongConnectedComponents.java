package cycle;

import java.util.Vector;
public class StrongConnectedComponents {
	
	private int[][] adjListOriginal = null;
	private int[][] adjList = null;
	private boolean[] visited = null;
	@SuppressWarnings("rawtypes")
	private Vector stack = null;
	private int[] lowlink = null;
	private int[] number = null;
	private int sccCounter = 0;
	@SuppressWarnings("rawtypes")
	private Vector currentSCCs = null;
	public StrongConnectedComponents(int[][] adjList) {
		this.adjListOriginal = adjList;
	}

	@SuppressWarnings("rawtypes")
	public SCCResult getAdjacencyList(int node) {
		this.visited = new boolean[this.adjListOriginal.length];
		this.lowlink = new int[this.adjListOriginal.length];
		this.number = new int[this.adjListOriginal.length];
		this.visited = new boolean[this.adjListOriginal.length];
		this.stack = new Vector();
		this.currentSCCs = new Vector();

		this.makeAdjListSubgraph(node);

		for (int i = node; i < this.adjListOriginal.length; i++) {
			if (!this.visited[i]) {
				this.getStrongConnectedComponents(i);
				Vector nodes = this.getLowestIdComponent();
				if (nodes != null && !nodes.contains(new Integer(node)) && !nodes.contains(new Integer(node + 1))) {
					return this.getAdjacencyList(node + 1);
				} else {
					Vector[] adjacencyList = this.getAdjList(nodes);
					if (adjacencyList != null) {
						for (int j = 0; j < this.adjListOriginal.length; j++) {
							if (adjacencyList[j].size() > 0) {
								return new SCCResult(adjacencyList, j);
							}
						}
					}
				}
			}
		}

		return null;
	}

	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	private void makeAdjListSubgraph(int node) {
		this.adjList = new int[this.adjListOriginal.length][0];

		for (int i = node; i < this.adjList.length; i++) {
			Vector successors = new Vector();
			for (int j = 0; j < this.adjListOriginal[i].length; j++) {
				if (this.adjListOriginal[i][j] >= node) {
					successors.add(new Integer(this.adjListOriginal[i][j]));
				}
			}
			if (successors.size() > 0) {
				this.adjList[i] = new int[successors.size()];
				for (int j = 0; j < successors.size(); j++) {
					Integer succ = (Integer) successors.get(j);
					this.adjList[i][j] = succ.intValue();
				}
			}
		}
	}

	@SuppressWarnings("rawtypes")
	private Vector getLowestIdComponent() {
		int min = this.adjList.length;
		Vector currScc = null;

		for (int i = 0; i < this.currentSCCs.size(); i++) {
			Vector scc = (Vector) this.currentSCCs.get(i);
			for (int j = 0; j < scc.size(); j++) {
				Integer node = (Integer) scc.get(j);
				if (node.intValue() < min) {
					currScc = scc;
					min = node.intValue();
				}
			}
		}

		return currScc;
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	private Vector[] getAdjList(Vector nodes) {
		Vector[] lowestIdAdjacencyList = null;

		if (nodes != null) {
			lowestIdAdjacencyList = new Vector[this.adjList.length];
			for (int i = 0; i < lowestIdAdjacencyList.length; i++) {
				lowestIdAdjacencyList[i] = new Vector();
			}
			for (int i = 0; i < nodes.size(); i++) {
				int node = ((Integer) nodes.get(i)).intValue();
				for (int j = 0; j < this.adjList[node].length; j++) {
					int succ = this.adjList[node][j];
					if (nodes.contains(new Integer(succ))) {
						lowestIdAdjacencyList[node].add(new Integer(succ));
					}
				}
			}
		}

		return lowestIdAdjacencyList;
	}

	@SuppressWarnings({ "unchecked", "rawtypes" })
	private void getStrongConnectedComponents(int root) {
		this.sccCounter++;
		this.lowlink[root] = this.sccCounter;
		this.number[root] = this.sccCounter;
		this.visited[root] = true;
		this.stack.add(new Integer(root));

		for (int i = 0; i < this.adjList[root].length; i++) {
			int w = this.adjList[root][i];
			if (!this.visited[w]) {
				this.getStrongConnectedComponents(w);
				this.lowlink[root] = Math.min(lowlink[root], lowlink[w]);
			} else if (this.number[w] < this.number[root]) {
				if (this.stack.contains(new Integer(w))) {
					lowlink[root] = Math.min(this.lowlink[root], this.number[w]);
				}
			}
		}

		// found scc
		if ((lowlink[root] == number[root]) && (stack.size() > 0)) {
			int next = -1;
			Vector scc = new Vector();

			do {
				next = ((Integer) this.stack.get(stack.size() - 1)).intValue();
				this.stack.remove(stack.size() - 1);
				scc.add(new Integer(next));
			} while (this.number[next] > this.number[root]);

			if (scc.size() > 1) {
				this.currentSCCs.add(scc);
			}
		}
	}
}
