package il.ac.tau.cs.sw1.ex9.starfleet;

import java.util.Set;

public class AbstSpaceship implements Spaceship {

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + AMC;
		result = prime * result + CoYear;
		result = prime * result + FirePower;
		result = prime * result + Float.floatToIntBits(MaxSpeed);
		result = prime * result + ((members == null) ? 0 : members.hashCode());
		result = prime * result + ((name == null) ? 0 : name.hashCode());
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
		AbstSpaceship other = (AbstSpaceship) obj;
		if (AMC != other.AMC)
			return false;
		if (CoYear != other.CoYear)
			return false;
		if (FirePower != other.FirePower)
			return false;
		if (Float.floatToIntBits(MaxSpeed) != Float.floatToIntBits(other.MaxSpeed))
			return false;
		if (members == null) {
			if (other.members != null)
				return false;
		} else if (!members.equals(other.members))
			return false;
		if (name == null) {
			if (other.name != null)
				return false;
		} else if (!name.equals(other.name))
			return false;
		return true;
	}

	private String name;
	private int CoYear;
	protected int FirePower;
	protected int AMC;
	protected float MaxSpeed;
	private Set<CrewMember> members;
	
	public AbstSpaceship(String name, int CoYear, float MaxSpeed, Set<CrewMember> members) {
		this.name = name;
		this.CoYear = CoYear;
		this.MaxSpeed = MaxSpeed;
		this.members = members;
		this.FirePower = 10;
	}
	
	@Override
	public String getName() {
		return name;
	}

	@Override
	public int getCommissionYear() {
		return CoYear;
	}

	@Override
	public float getMaximalSpeed() {
		return MaxSpeed;
	}

	@Override
	public int getFirePower() {
		return FirePower;
	}

	@Override
	public Set<CrewMember> getCrewMembers() {
		return members;
	}

	@Override
	public int getAnnualMaintenanceCost() {
		return AMC;
	}
	
	public String toString(){
		String sp = System.lineSeparator();
		return this.getClass().getSimpleName()+sp+"\t"+"Name="+getName()+
				sp+"\t"+"CommissionYear="+getCommissionYear()+
				sp+"\t"+"MaximalSpeed="+getMaximalSpeed()+
				sp+"\t"+"FirePower="+getFirePower()+
				sp+"\t"+"CrewMembers="+getCrewMembers().size()+
				sp+"\t"+"AnnualMaintenanceCost="+getAnnualMaintenanceCost();
	}

	@Override
	public int compareTo(Spaceship other) {
		int i = this.getFirePower() - other.getFirePower();
		if (i != 0)
			return -i;
		i = this.getCommissionYear() - other.getCommissionYear();
		if (i != 0)
			return -i;
		return this.name.compareTo(other.getName());
	}
}
