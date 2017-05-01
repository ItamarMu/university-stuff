package sw1.polynomials;

/**
 * Represents a multiplication of variables in a-z with an integral coefficient
 */
public class Monomial {
	
	private int [] letrs; //each cell represents letter^(cell value)
	private int coef;
	/**
	 * @post this.getCoefficient() == coefficient
	 * @post for every v, 'a'<=v<='z', isVariable(v) == false
	 */
	public Monomial(int coefficient) {
		coef=coefficient;
		letrs=new int[26];
	}

	/**
	 * @return the coefficient of this monomial
	 */
	public int getCoefficient() {
		return coef;
	}

	/**
	 * @post getCoefficient() == coefficient
	 */
	public void setCoefficient(int coefficient) {
		coef=coefficient;
	}

	/**
	 * @return true iff the input is a variable of this monomial (and appears in
	 *         toString).
	 */
	public boolean isVariable(char variable) {
		int ascval=(int)variable;
		return letrs[ascval-97]!=0;
	}

	/**
	 * @pre isVariable(variable)
	 * @return the degree of variable in this monomial
	 */
	public int getDegree(char variable) {
		int ascval=(int)variable;
		return letrs[ascval-97];
	}

	/**
	 * @pre degree >= 0
	 * @pre 'a'<=variable<='z'
	 * @post getDegree(variable) = degree
	 */
	public void setDegree(char variable, int degree) {
		int ascval=(int)variable;
		letrs[ascval-97]=degree;
	}

	/**
	 * @pre other!= null
	 * @return true iff the set of variables and the degree of each variable is
	 *         the same for this and other.
	 */
	public boolean hasSameDegrees(Monomial other) {
		for (int i=0; i<this.letrs.length; i++){
			if (this.letrs[i]!=other.letrs[i])
				return false;
		}
		return true;
	}
	
	/**
	 * @pre assignment != null
	 * @pre assignment.length == 26
	 * @return the result of assigning assignment[0] to a, assignment[1] to b
	 *         etc., and computing the value of this Monomial
	 */
	public int evaluate(int[] assignment) { //multiply just variables without coef
		int sum=1; 
		for (int i=0; i<letrs.length; i++){
			sum*=(int)Math.pow(assignment[i],letrs[i]);
		}
		return coef*sum;
	}

	/**
	 * Returns a string representation of this monomial by the mathematical
	 * convention. I.e., the coefficient is first (if not 1), then every
	 * variable in an alphabetic order followed by ^ and its degree (if > 1).
	 * For example, 13b^2x^3z
	 */
	public String toString() {
		StringBuilder str = new StringBuilder();
		for (int i=0; i<letrs.length; i++){
			if (letrs[i]!=0){
				str.append((char)(i+97));
				if (letrs[i]!=1)
					str.append("^"+letrs[i]);
			}
		}
		if (coef!=1 && coef!=-1)
			return coef+str.toString();
		if (coef==1)
			return str.toString();
		return "-"+str.toString();
	}
	
	/**
	 * Returns a "safe" copy of this monomial, i.e., if the copy is changed,
	 * this will not change and vice versa
	 */
	public Monomial getCopy() {
		Monomial cop = new Monomial(coef);
		for (int i=0; i<letrs.length; i++)
			cop.setDegree((char)(i+97), letrs[i]);
		return cop;
	}
	
	public Monomial multiply1(Monomial other) { 
		Monomial mul = new Monomial(coef*other.coef);
		for (int i=0; i<mul.letrs.length; i++){
			char letter=(char)(i+97);
			mul.setDegree(letter, this.getDegree(letter)+other.getDegree(letter));
		}
		return mul;
	}
}
