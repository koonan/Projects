package cycle;

import java.util.Collections;
import java.util.List;
import java.util.Vector;

public class AllCycle {

	private boolean[][] adjMatrix;
	private double[][] val;
	private String[] nodes;
	private CyclesSearch ecs;
	private List<Object> cycles;
	private List<Object> selfLoop;
	private List<Object> combination;
	private List<Object> allCost;
	private boolean[][] check;
	private int size = 0;

	@SuppressWarnings({ "unchecked", "rawtypes" })
	public AllCycle(double[][] adjMatrix) {

		val = adjMatrix;
		selfLoop = new Vector();
		allCost = new Vector();
		this.combination = new Vector();
		this.adjMatrix = new boolean[val.length][val[0].length];
		this.check = new boolean[100][100];
		nodes = new String[val.length];
		for (int i = 0; i < val.length; i++) {
			for (int j = 0; j < val[0].length; j++) {
				if (val[i][j] != 0) {
					this.adjMatrix[i][j] = true;
					if (i == j) {
						this.selfLoop.add(i);
					}
				}

			}

		}
		for (int i = 0; i < this.val.length; i++) {
			Integer no = i;
			nodes[i] = no.toString();
		}
		ecs = new CyclesSearch(this.adjMatrix, nodes);
		this.cycles = ecs.getElementaryCycles();

		for (int i = 0; i < cycles.size(); i++) {
			List<Object> cycle = (List<Object>) cycles.get(i);
			if (cycle.size() == 1) {
				cycles.remove(i);
			}
		}
		for (int i = 0; i < this.selfLoop.size(); i++) {
			List<Object> cycle = new Vector();
			cycle.add(selfLoop.get(i).toString());
			this.cycles.add(cycle);

		}

		if (this.cycles.size() != 0) {

			int count = 0;

			this.allCost.add(calculateCost(cycles));
			this.combination.add(this.cycles);

			while (count == this.size) {

				boolean found = false;
				count++;
				List<Object> newCombination = new Vector();
				List<Object> newCombination2 = new Vector();
				List<Object> newCost = new Vector();
				List group = (List) this.combination.get(size);
				for (int i = 0; i < cycles.size(); i++) {
					List cycle = (List) cycles.get(i);
					int c = 0;
					if (size == 0) {
						c = i + 1;
					}
					for (int k = c; k < group.size(); k++) {

						List cycle2 = (List) group.get(k);
						boolean escape = false;
						for (int j = 0; j < cycle.size() && !found && !escape; j++) {
							String node;
							if (size != 0) {
								node = Integer.toString(i);
								escape = true;
							} else {
								node = (String) cycle.get(j);
							}

							for (int x = 0; x < cycle2.size() && !found; x++) {
								String node2 = (String) cycle2.get(x);
								if (node2.equals(",")) {
									continue;
								}
								if (size == 0) {
									if (node.equals(node2)) {
										found = true;
									}
								} else {

									if (!check[Integer.parseInt(node2)][Integer
											.parseInt(node)]) {
										found = true;
									}
								}

							}
						}
						if (!found) {
							
							List<Object> nontouch = new Vector();
							List<Object> nontouch2 = new Vector();
							if (size != 0) {

								nontouch.add(Integer.toString(i));
								nontouch2.add(Integer.toString(i));
								nontouch2.add(",");
								for (int v = 0; v < cycle2.size(); v++) {
									if (cycle2.get(v).equals(",")) {
										nontouch2.add(cycle2.get(v));
									} else {
										nontouch.add(cycle2.get(v));
										nontouch2.add(cycle2.get(v));
									}

								}

							} else {
								nontouch.add(Integer.toString(i));
								nontouch.add(Integer.toString(k));
								nontouch2.add(Integer.toString(i));
								nontouch2.add(",");
								nontouch2.add(Integer.toString(k));
								check[i][k] = true;
								check[k][i] = true;
							}

							List cost1 = (List) this.allCost.get(0);
							double c1 = (double) cost1.get(i);
							List cost2 = (List) this.allCost.get(size);
							double c2 = (double) cost2.get(k);
							
							Collections.sort((List) nontouch);
							boolean repeated = false;
							for (int s = 0; s < newCombination.size()
									&& !repeated && size != 0; s++) {
								List list = (List) newCombination.get(s);
								if (list.equals(nontouch)) {
									repeated = true;

								}
							}
							if (!repeated) {
								newCombination.add(nontouch);
								newCombination2.add(nontouch2);
								newCost.add(c2 * c1);
							}

						} else {
							found = false;
						}
					}
				}
				if (newCombination.size() != 0) {

					this.combination.add(newCombination2);
					this.allCost.add(newCost);
					size++;
				}
			}
	
		}

	
	}

	private List<Object> calculateCost(List<Object> cycles) {
		List<Object> cost = new Vector<Object>();
		String n3;
		int node3;
		for (int i = 0; i < cycles.size(); i++) {
			@SuppressWarnings("rawtypes")
			List cycle = (List) cycles.get(i);
			double c = 1;
			if (cycle.size() == 1) {
				String n1 = (String) cycle.get(0);
				int node1 = Integer.parseInt(n1);
				c = c * val[node1][node1];
			}
			for (int j = 0; j < cycle.size() - 1; j++) {

				String n1 = (String) cycle.get(j);
				String n2 = (String) cycle.get(j + 1);
				int node1 = Integer.parseInt(n1);
				int node2 = Integer.parseInt(n2);
				if (j == 0) {
					n3 = (String) cycle.get(cycle.size() - 1);
					node3 = Integer.parseInt(n3);
					c = c * val[node3][node1];
				}
				c = c * val[node1][node2];

			}
			cost.add(c);
		}
		return cost;
	}

	public List<Object> getCombination() {
		return this.combination;
	}

	public List<Object> getAllCost() {
		return this.allCost;
	}

}
