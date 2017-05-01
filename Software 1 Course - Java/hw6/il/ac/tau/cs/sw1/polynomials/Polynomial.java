package sw1.polynomials;

public class Polynomial {

	/**
	 * Creates a polynomial with (safe copies of) the given monomials
	 * 
	 * @pre monomials != null
	 * @pre for all i, 0 <= i < monmials.length : monomials[i] != null
	 * @post for all i, 0 <= i < monmials.length : monomials[i].getCoefficient()
	 *       == getMonomial(i).getCoefficient()
	 * @post for all i,v, 0 <= i < monmials.length, 'a'<=v<='z' :
	 *       monomials[i].getDegree(v) == getMonomial(i).getDegree(v)
	 */
	private Monomial[] monos;
	
	public Polynomial(Monomial[] monomials) {
		monos = new Monomial[monomials.length];
		for (int i=0; i<monomials.length; i++){
			monos[i]=monomials[i].getCopy();
		}
	}

	/**
	 * @return the number of monomials in this polynomial
	 */
	public int getMonomialCount() {
		return monos.length;
	}

	/**
	 * @pre 0<=index < getMonomialCount()
	 * @return a safe copy of the monomial at the given index
	 */
	public Monomial getMonomial(int index) {
		return monos[index].getCopy();
	}

	/**
	 * @pre other != null
	 * @post Creates a new Polynomial which is the sum of this polynomial and
	 *       other. E.g., the sum of 13b^2x^3z+15 and -4b^2x^3z is
	 *       9b^2x^3z+15
	 */
	public Polynomial add(Polynomial other) {
		Monomial[] newp = new Monomial[monos.length+other.monos.length];
		for (int i=0; i<monos.length; i++)
			newp[i]=monos[i].getCopy();
		for (int i=0; i<other.monos.length; i++)
			newp[monos.length+i]=other.monos[i].getCopy();
		for (int i=0; i<newp.length; i++){
			for (int j=i+1; j<newp.length; j++){
				if (newp[i].hasSameDegrees(newp[j])){
					newp[i].setCoefficient(newp[i].getCoefficient()+newp[j].getCoefficient());
					newp[j].setCoefficient(0);
				}
			}
		}
		int cntnotzero=0, ind=0;;
		for (int i=0; i<newp.length; i++)
			if (newp[i].getCoefficient()!=0)
				cntnotzero++;
		if (cntnotzero==0){
			Monomial m = new Monomial(0);
			Monomial [] m1 = {m};
			Polynomial ret = new Polynomial(m1);
			return ret;
		}
		Monomial[] newp1 = new Monomial[cntnotzero];
		for (int i=0; i<newp.length; i++)
			if (newp[i].getCoefficient()!=0)
				newp1[ind++]=newp[i];
		Polynomial ret = new Polynomial(newp1);
		return ret;
	}
	/**
	 * @pre other != null
	 * @post Creates a new Polynomial which is the product of this polynomial
	 *       and other. E.g., the product of 13b^2x^3z+15 and -4b^2x^3z is
	 *       -52b^4x^6z^2-60b^2x^3z
	 */
	public Polynomial multiply(Polynomial other) {
		Monomial[] newp =new Monomial[monos.length*other.monos.length];
		int ind=0;
		for (int i=0; i<monos.length; i++)
			for (int j=0; j<other.monos.length; j++)
				newp[ind++]=this.getMonomial(i).multiply1(other.getMonomial(j));
		Monomial m = new Monomial(0);
		Monomial [] m1 = {m};
		Polynomial m2 = new Polynomial(m1);
		Polynomial m3 = new Polynomial(newp);
		return m3.add(m2);
		}

	/**
	 * @pre assignment != null
	 * @pre assignment.length == 26
	 * @return the result of assigning assignment[0] to a, assignment[1] to b
	 *         etc., and computing the value of this Polynomial
	 */
	public int evaluate(int[] assignment) {
		int sum=0;
		for (int i=0; i<monos.length; i++)
			sum+=monos[i].evaluate(assignment);
		return sum;
	}


	/**
	 * Returns a string representation of this polynomial by the mathematical
	 * convention, but without performing normalization (summing of monomials).
	 * I.e., each monomial is printed according to Monomial.toString(), for
	 * example 13b^2x^3z+15-4b^2x^3z
	 */
	public String toString() {
		StringBuffer str = new StringBuffer();
		str.append(monos[0]);
		for (int i=1; i<monos.length; i++){
			if (monos[i].getCoefficient()>=0)
				str.append("+"+monos[i]);
			else
				str.append(monos[i]);
		}
		return str.toString();
	}
}
