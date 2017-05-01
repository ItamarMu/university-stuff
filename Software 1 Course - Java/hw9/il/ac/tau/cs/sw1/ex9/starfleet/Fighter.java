package il.ac.tau.cs.sw1.ex9.starfleet;

import java.util.List;
import java.util.Set;

public class Fighter extends AbstBattleship{

	public Fighter(String name, int CoYear, float MaxSpeed, Set<CrewMember> members, List<Weapon> wpns) {
		super(name, CoYear, MaxSpeed, members, wpns);
		this.AMC = 2500;
	}

	public int getAnnualMaintenanceCost(){
		int sum=0;
		for (Weapon wp:wpns)
			sum+=wp.getAnnualMaintenanceCost();
		return super.getAnnualMaintenanceCost()+sum+(int)(1000*this.MaxSpeed);
	}
}
