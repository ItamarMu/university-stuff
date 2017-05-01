package riddles;

public class F implements Comparable<F>{

	private int num1;
	private int num2;
	private static int cnt = 0;
	private int c;
	
	public F (int num1, int num2){
		this.num1 = num1;
		this.num2 = num2;
		cnt++;
		c = cnt;
	}
	
	public int compareTo (F other){
		if (this.num1 == other.num1 && this.num2 == other.num2 && this.c == other.c)
			return 0;
		return 1;
	}
}
