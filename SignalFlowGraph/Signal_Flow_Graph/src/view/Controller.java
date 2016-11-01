package view;

import java.util.ArrayList;

public class Controller {

	public Mason mason;
	public ArrayList<String> nodesNames;

	public Controller() {
		this.nodesNames = DrawPanel.nodesNames;
		this.mason = new Mason(nodesNames);
	}

	public void addNode() {
		mason.addNode();
	}

	public void addBranch(int from, int to, double value) {
		mason.addBranch(from, to, value);
	}

	public void getStartAndEnd(int[] startAndEnd,String start,String end)
    {
           for(int i=0 ; i < nodesNames.size() ; i++)
           {
               if(nodesNames.get(i).equals(start))  startAndEnd[0]=i;
               else if(nodesNames.get(i).equals(end)) startAndEnd[1]= i;               
           }   
    }
	public ArrayList<ArrayList<Integer>> getForwardPaths(String startNode,String endNode) {
		int[] arr = new int[2];
		getStartAndEnd(arr, startNode, endNode);
		return mason.getForwardPaths(arr[0],arr[1]);
	}

	public String getForwardPathsNames(String startNode,String endNode) {

		ArrayList<ArrayList<Integer>> forwardPathIndeces = getForwardPaths(startNode,endNode);
		String path = "";
		for (int i = 0; i < forwardPathIndeces.size(); i++) {
			path += (i + 1) + " : ";
			for (int j = 0; j < forwardPathIndeces.get(i).size(); j++)
				path += nodesNames.get(forwardPathIndeces.get(i).get(j)) + " ";

			path += "\n";
		}

		return path;
	}



}
