package cycle;


import java.util.Vector;

public class SCCResult {
	@SuppressWarnings("rawtypes")
	private Vector[] adjList = null;
	private int lowestNodeId = -1;
	
	@SuppressWarnings("rawtypes")
	public SCCResult(Vector[] adjList, int lowestNodeId) {
		this.adjList = adjList;
		this.lowestNodeId = lowestNodeId;
	}

	@SuppressWarnings("rawtypes")
	public Vector[] getAdjList() {
		return adjList;
	}

	public int getLowestNodeId() {
		return lowestNodeId;
	}
}
