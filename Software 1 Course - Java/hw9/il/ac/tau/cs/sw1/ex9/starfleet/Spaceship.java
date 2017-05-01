package il.ac.tau.cs.sw1.ex9.starfleet;

import java.util.Set;

public interface Spaceship extends Comparable<Spaceship>{
	
	public String getName();
	
	public int getCommissionYear();
	
	public float getMaximalSpeed();
	
	public int getFirePower();
	
	public Set<CrewMember> getCrewMembers();
	
	public int getAnnualMaintenanceCost();

	int compareTo(Spaceship other);
	
}
