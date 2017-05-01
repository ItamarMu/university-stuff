package riddles;

public class E {

	private int num1;
	private int num2;
	private static int cnt = 0;
	private int c;
	
	public E (int num1, int num2){
		this.num1 = num1;
		this.num2 = num2;
		cnt++;
		c = cnt;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + c;
		result = prime * result + num1;
		result = prime * result + num2;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		E other = (E) obj;
		if (c != other.c)
			return false;
		if (num1 != other.num1)
			return false;
		if (num2 != other.num2)
			return false;
		return true;
	}
}
