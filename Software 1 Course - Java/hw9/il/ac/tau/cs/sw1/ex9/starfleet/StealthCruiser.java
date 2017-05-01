package il.ac.tau.cs.sw1.ex9.starfleet;


import java.util.Arrays;
import java.util.List;
import java.util.Set;

public class StealthCruiser extends Fighter{

	private static int numOfStealthCruisers=0;
	
	public StealthCruiser(String name, int CoYear, float MaxSpeed, Set<CrewMember> members, List<Weapon> wpns) {
		super(name, CoYear, MaxSpeed, members, wpns);
		numOfStealthCruisers++;
	}
	
	public StealthCruiser(String name, int CoYear, float MaxSpeed, Set<CrewMember> members) {
		this(name, CoYear, MaxSpeed, members, Arrays.asList(new Weapon("Laser Cannons",10,100)));
	}

	public int getAnnualMaintenanceCost(){
		return super.getAnnualMaintenanceCost()+100*numOfStealthCruisers;
	}
}
