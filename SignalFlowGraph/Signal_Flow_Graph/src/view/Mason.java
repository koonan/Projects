package view;

import java.util.ArrayList;
import java.util.List;

import cycle.AllCycle;

class Mason {
	public ArrayList<String> nodesNames;
	private ArrayList<ArrayList<MyEntry<Integer, Double>>> signalFlowGraph;
	private ArrayList<Boolean> visited;
	public AllCycle cycle ;
	public ArrayList<ArrayList<Integer>> forwardPathsIndices;
	public ArrayList<String> forwardPaths;
	public double[] forwardPathGains;
	public int source, sink, parent;
	private String deltaName ;
	private double numerator ;
	private double denominator;
	private double[][] array ;
	private boolean check[][];

	public Mason(ArrayList<String> names) {
		this.nodesNames = names;
		this.deltaName = "delta";
		this.numerator = 0;
		this.denominator = 0;
		signalFlowGraph = new ArrayList<ArrayList<MyEntry<Integer, Double>>>();
		visited = new ArrayList<Boolean>();
		
	}

	public void findLoop(){
		this.array = this.get2D();
		this.cycle = new AllCycle(this.array);
		
	}
	public void addNode() {
		signalFlowGraph.add(new ArrayList<MyEntry<Integer, Double>>());
		visited.add(false);
		ScrollablePanel.draw.drawing();

	}
	public String findDelta(){
		this.denominator = 1;
		for(int i = 0 ; i < this.cycle.getAllCost().size();i++){
			@SuppressWarnings("rawtypes")
			List group = (List)this.cycle.getAllCost().get(i);
			double cost = 0;
			for(int j = 0 ; j < group.size() ; j++){
				cost += (double)group.get(j);
			}
			if(i%2 == 0){
				this.denominator -= cost;
			}
			else{
				this.denominator += cost;
			}
		}
		String deltaString = Double.toString(this.denominator);
		return deltaString;
		
	}
	public String findTF(){
		double transferFN = this.numerator/this.denominator;
		String transferFNString = Double.toString(transferFN);
		return transferFNString;
	}
	@SuppressWarnings("rawtypes")
	public String findDeltan(){
		this.numerator = 0;
		boolean found = false;
		String deltan = " ";
		this.check = new boolean[forwardPathsIndices.size()][100];
		for (int i = 0; i < forwardPathsIndices.size(); i++) {

			List path = (List) forwardPathsIndices.get(i);
			
			double delta = 1;
			
			for (int k = 0; k < this.cycle.getCombination().size(); k++) {

				double cost = 0;
				List group = (List) this.cycle.getCombination().get(k);
				for(int g = 0 ; g < group.size(); g++){ 
					List cycle = (List)group.get(g);
				    boolean escape = false;
				for (int j = 0; j < path.size() && !found && !escape; j++) {
					//int index = (int) path.get(j);
					String node = null;
					if(k != 0){
						escape = true;
					}
					else{
						 node = Integer.toString((int) path.get(j));
					}
					
					for (int x = 0; x < cycle.size() && !found; x++) {
						String node2 = (String) cycle.get(x);
						if(node2.equals(",")){
							continue;
						}
						if(k == 0){
							if (node.equals(node2)) {
								found = true;
							}
						}
						else{
							if(!check[i][Integer.parseInt(node2)]){
								found = true;
							}
						}
						
					}
				}
				
				if (!found) {
					if(k == 0){
						this.check[i][g] = true;
					}
					List list = (List)this.cycle.getAllCost().get(k);
					cost += (double)list.get(g);
						
				}
				else{
					found = false;
				}
			
			}
			if(k %2 == 0){
				delta = delta - cost;
			}
			else{
				delta = delta + cost;
			}
		}
			String deltaString = Double.toString(delta);
			this.numerator += delta * this.forwardPathGains[i];
			deltan +=  deltaName + " " + (i+1)+"  :" + deltaString+"\n";
			
	}
		return deltan;
	}
	

	public void addBranch(int from, int to, double value) {
		signalFlowGraph.get(from).add(new MyEntry<Integer, Double>(to, value));
	}

	public ArrayList<ArrayList<Integer>> getForwardPaths(int startNode, int endNode) {
		getForwadPath(startNode, endNode);
		return forwardPathsIndices;
	}

	public void getForwadPath(int start, int end) {
		get2D();
		forwardPaths = new ArrayList<String>();
		source = start;
		sink = end;
		visited.set(source, true);
		find(source, "" + source);
		forwardPathsIndices = new ArrayList<ArrayList<Integer>>();
		splitPaths();
		getForwardPathGain();

	}

	public void find(int s, String path) {
		if (s == sink) {
			forwardPaths.add(path);
			return;

		}
		parent = s;
		for (MyEntry<Integer, Double> i : signalFlowGraph.get(s)) {
			if (!visited.get(i.getFirst())) {
				visited.set(i.getFirst(), true);
				find(i.getFirst(), path + ", " + i.getFirst());
				parent = s;
				visited.set(i.getFirst(), false);

			}
		}

	}

	public void splitPaths() {
		for (int i = 0; i < forwardPaths.size(); i++) {
			String[] splitter = forwardPaths.get(i).split(", ");
			forwardPathsIndices.add((new ArrayList<Integer>()));
			for (int j = 0; j < splitter.length; j++) {
				forwardPathsIndices.get(i).add(Integer.parseInt(splitter[j]));
			}
		}
	}

	public void getForwardPathGain() {
		forwardPathGains = new double[forwardPathsIndices.size()];
		double pathGain = 1;
		for (int i = 0; i < forwardPathsIndices.size(); i++) {
			pathGain = 1;
			for (int j = 0; j < forwardPathsIndices.get(i).size() - 1; j++) {
				pathGain *= branchGain(forwardPathsIndices.get(i).get(j), forwardPathsIndices.get(i).get(j + 1));
			}
			forwardPathGains[i] = pathGain;

		}

	}

	public double branchGain(int start, int end) {
		for (int i = 0; i < signalFlowGraph.get(start).size(); i++) {
			if (signalFlowGraph.get(start).get(i).getFirst() == end) {
				return signalFlowGraph.get(start).get(i).getSecond();

			}
		}
		return 0;
	}

	public double[][] get2D() {
		double[][] arr = new double[signalFlowGraph.size()][signalFlowGraph.size()];

		for (int i = 0; i < signalFlowGraph.size(); i++) {
			for (int j = 0; j < signalFlowGraph.get(i).size(); j++) {
				arr[i][signalFlowGraph.get(i).get(j).getFirst()] = signalFlowGraph.get(i).get(j).getSecond();
			}
		}
		

		return arr;
	}

}
