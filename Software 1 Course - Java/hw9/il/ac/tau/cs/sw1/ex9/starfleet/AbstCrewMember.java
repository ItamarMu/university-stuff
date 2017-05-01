package il.ac.tau.cs.sw1.ex9.starfleet;

public abstract class AbstCrewMember implements CrewMember {
	
	private String name;
	private int age;
	private int YIS;
	
	public AbstCrewMember(String name, int age, int YIS){
		this.name=name;
		this.age=age;
		this.YIS=YIS;
	}

	public String getName() {
		return name;
	}

	public int getAge() {
		return age;
	}

	public int getYearsInService() {
		return YIS;
	}

	public int compareTo(CrewMember other){
		return this.name.compareTo(other.getName());
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + YIS;
		result = prime * result + age;
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
		AbstCrewMember other = (AbstCrewMember) obj;
		if (YIS != other.YIS)
			return false;
		if (age != other.age)
			return false;
		if (name == null) {
			if (other.name != null)
				return false;
		} else if (!name.equals(other.name))
			return false;
		return true;
	}
}
