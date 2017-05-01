package riddles;

public class B {
	
	private int num1;
	
	public B (int num1, int num2){
		this.num1 = num1;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + num1;
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
		B other = (B) obj;
		if (num1 != other.num1)
			return false;
		return true;
	}
}
