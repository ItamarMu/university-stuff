package riddles;

public class D implements Comparable<D>{

	private int num1;
	
	public D (int num1, int num2){
		this.num1 = num1;
	}

	public int compareTo (D other){
		if (this.num1 == other.num1)
			return 0;
		return 1;
	}
}
