package riddles;

public class C implements Comparable<C>{

	private int num1;
	private int num2;
	
	public C (int num1, int num2){
		this.num1 = num1;
		this.num2 = num2;
	}

	@Override
	public int compareTo(C other) {
		if (this.num1 == other.num1 && this.num2 == other.num2)
			return 0;
		return 1;
	}
}
