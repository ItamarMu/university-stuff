package il.ac.tau.cs.sw1.ex9.starfleet;

public class Officer extends AbstCrewMember{
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = super.hashCode();
		result = prime * result + ((oRank == null) ? 0 : oRank.hashCode());
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
		Officer other = (Officer) obj;
		if (oRank != other.oRank)
			return false;
		return true;
	}

	private OfficerRank oRank;
	
	public Officer(String name, int age, int YIS, OfficerRank oRank) {
		super(name, age, YIS);
		this.oRank=oRank;
	}
	
	public OfficerRank getRank(){
		return oRank;
	}
	
	@Override
	public int compareTo(CrewMember other){
		if (other instanceof CrewWoman)
			return 1;
		int i = this.getRank().compareTo(((Officer) other).getRank());
		if (i != 0)
			return -i;
		return super.compareTo(other);
	}
}
