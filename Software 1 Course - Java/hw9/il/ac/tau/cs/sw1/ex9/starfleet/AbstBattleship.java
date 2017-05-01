package il.ac.tau.cs.sw1.ex9.starfleet;

import java.util.List;
import java.util.Set;

public class AbstBattleship extends AbstSpaceship {
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = super.hashCode();
		result = prime * result + ((wpns == null) ? 0 : wpns.hashCode());
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
		AbstBattleship other = (AbstBattleship) obj;
		if (wpns == null) {
			if (other.wpns != null)
				return false;
		} else if (!wpns.equals(other.wpns))
			return false;
		return true;
	}

	protected List<Weapon> wpns;

	public AbstBattleship(String name, int CoYear, float MaxSpeed, Set<CrewMember> members, List<Weapon> wpns) {
		super(name, CoYear, MaxSpeed, members);
		this.wpns = wpns;
	}
	
	public List<Weapon> getWeapon(){
		return wpns;
	}
	
	public int getFirePower(){
		int sum=0;
		for (Weapon wpn:wpns){
			sum+=wpn.getFirePower();
		}
		return this.FirePower+sum;
	}
	
	public String toString(){
		String sp = System.lineSeparator();
		return super.toString() +
				sp+"\t"+"WeaponArray="+wpns.toString();
	}
}
