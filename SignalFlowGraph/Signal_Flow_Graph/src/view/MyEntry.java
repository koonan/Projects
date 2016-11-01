package view;

public class MyEntry<f, s> {
	private f first;
	private s second;

	public MyEntry(f first, s second) {
		this.first = first;
		this.second = second;

	}

	public f getFirst() {
		return this.first;
	}

	public s getSecond() {
		return this.second;
	}

	public void setFirst(f value) {
		this.first = value;
	}

	public void setSecond(s value) {
		this.second = value;
	}

}
