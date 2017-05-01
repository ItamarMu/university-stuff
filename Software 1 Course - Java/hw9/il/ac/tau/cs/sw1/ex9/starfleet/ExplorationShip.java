package il.ac.tau.cs.sw1.ex9.starfleet;

import java.util.Set;

public class ExplorationShip extends AbstSpaceship{
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = super.hashCode();
		result = prime * result + SLabs;
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
		ExplorationShip other = (ExplorationShip) obj;
		if (SLabs != other.SLabs)
			return false;
		return true;
	}

	private int SLabs;
	
	public ExplorationShip(String name, int CoYear, float MaxSpeed,
			Set<CrewMember> members, int SLabs) {
		super(name, CoYear, MaxSpeed, members);
		this.SLabs=SLabs;
		this.AMC = 4000;
	}
	
	public int getNumberOfResearchLabs(){
		return SLabs;
	}
	
	public int getAnnualMaintenanceCost(){
		return super.getAnnualMaintenanceCost()+(2500*SLabs);
	}
	
	public String toString(){
		String sp = System.lineSeparator();
		return super.toString() +
		sp+"\t"+"NumberOfResearchLabs="+SLabs;
	}
}
