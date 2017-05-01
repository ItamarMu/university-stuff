package il.ac.tau.cs.sw1.ex9.starfleet;

import java.util.List;
import java.util.Set;

public class Bomber extends AbstBattleship{
	
	private int numOfTech;
	
	public Bomber(String name, int CoYear, float MaxSpeed, Set<CrewMember> members, List<Weapon> wpns, int numOfTech) {
		super(name, CoYear, MaxSpeed, members, wpns);
		this.numOfTech = numOfTech;
		this.AMC = 5000;
	}
	
	public List<Weapon> getWeapon(){
		return wpns;
	}
	
	public int getNumberOfTechnicians(){
		return numOfTech;
	}
	
	public int getAnnualMaintenanceCost(){
		int sum=0;
		for (Weapon wp:wpns)
			sum+=wp.getAnnualMaintenanceCost();
		return super.getAnnualMaintenanceCost()+(int)((10-numOfTech)*0.1*sum);
	}
	
	public String toString(){
		String sp = System.lineSeparator();
		return super.toString()+
				sp+"\t"+"NumberOfTechnicians="+numOfTech;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = super.hashCode();
		result = prime * result + numOfTech;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (!super.equals(obj))
			return false;
		if (getClass() != obj.getClass())
			return false;
		Bomber other = (Bomber) obj;
		if (numOfTech != other.numOfTech)
			return false;
		return true;
	}
}
