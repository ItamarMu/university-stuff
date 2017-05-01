package il.ac.tau.cs.sw1.ex9.starfleet;

public interface CrewMember extends Comparable<CrewMember>{
	
	public String getName();
	
	public int getAge();
	
	public int getYearsInService();
	
	int compareTo(CrewMember other);
}
