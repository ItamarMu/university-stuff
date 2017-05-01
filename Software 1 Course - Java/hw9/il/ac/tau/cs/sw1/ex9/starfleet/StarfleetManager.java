package il.ac.tau.cs.sw1.ex9.starfleet;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class StarfleetManager {

	/**
	 * Returns a list containing string representation of all fleet ships, sorted in descending order by
	 * fire power, and then in descending order by commission year
	 */
	public static List<String> getShipDescriptionsSortedByFirePowerAndCommissionYear(Collection<Spaceship> fleet) {
		List<Spaceship> tmp = new ArrayList<Spaceship>();
		tmp.addAll(fleet);
		Collections.sort(tmp);
		List<String> x = new ArrayList<String>();
		for (Spaceship ship:tmp)
			x.add(ship.toString());
		return x;
	}

	/**
	 * Returns a map containing ship type names as keys (the class name) and the number of instances created for each type as values
	 */
	public static Map<String, Integer> getInstanceNumberPerClass(Collection<Spaceship> fleet) {
		Map <String, Integer> hm = new HashMap<String, Integer>();
		for (Spaceship ship:fleet){
			String tp = ship.getClass().getSimpleName();
			if (hm.containsKey(tp))
				hm.put(tp, hm.get(tp)+1);
			else
				hm.put(tp, 1);
		}
		return hm;
	}


	/**
	 * Returns the total annual maintenance cost of the fleet (which is the sum of maintenance costs of all the fleet's ships)
	 */
	public static int getTotalMaintenanceCost (Collection<Spaceship> fleet) {
		int sum=0;
		for (Spaceship ship:fleet)
			sum+=ship.getAnnualMaintenanceCost();
		return sum;
	}


	/**
	 * Returns a set containing the names of all the fleet's weapons installed on any ship
	 */
	public static Set<String> getFleetWeaponNames(Collection<Spaceship> fleet) {
		Set <String> hs = new HashSet<String>();
		for (Spaceship ship:fleet)
			if (ship instanceof AbstBattleship)
				for (Weapon wp:((AbstBattleship) ship).getWeapon())
					hs.add(wp.getName());
		return hs;
	}

	/*
	 * Returns the total number of crew-members serving on board of the given fleet's ships.
	 */
	public static int getTotalNumberOfFleetCrewMembers(Collection<Spaceship> fleet) {
		int sum=0;
		for (Spaceship ship:fleet)
			sum+=ship.getCrewMembers().size();
		return sum;
	}

	/*
	 * Returns the average age of all officers serving on board of the given fleet's ships. 
	 */
	public static float getAverageAgeOfFleetOfficers(Collection<Spaceship> fleet) {
		float ageSum=0;
		int cnt=0;
		for (Spaceship ship:fleet)
			for (CrewMember mem:ship.getCrewMembers())
				if (mem instanceof Officer){
					cnt++;
					ageSum += mem.getAge();
				}
		return ageSum/cnt;
	}

	/*
	 * Returns a map mapping the highest ranking officer on each ship (as keys), to his ship (as values).
	 */
	public static Map<Officer, Spaceship> getHighestRankingOfficerPerShip(Collection<Spaceship> fleet) {
		Map <Officer, Spaceship> hm = new HashMap<Officer, Spaceship>();
		List <Officer> ls = new ArrayList<Officer>();
		for (Spaceship ship:fleet){
			for (CrewMember mem:ship.getCrewMembers()){
				if (mem instanceof Officer)
					ls.add((Officer)mem);
			}
			Collections.sort(ls);
			hm.put(ls.get(0), ship);
			ls.clear();
		}
		return hm;
	}

	/*
	 * Returns a List of entries representing ranks and their occurrences.
	 * Each entry represents a pair composed of an officer rank, and the number of its occurrences among starfleet personnel.
	 * The returned list is sorted descendingly based on the number of occurrences.
	 */
	public static List<Map.Entry<OfficerRank, Integer>> getOfficerRanksSortedByPopularity(Collection<Spaceship> fleet) {
		Map <OfficerRank, Integer> hm = new HashMap<OfficerRank, Integer>();
		for (Spaceship ship:fleet)
			for (CrewMember mem:ship.getCrewMembers())
				if (mem instanceof Officer){
					OfficerRank or = ((Officer) mem).getRank();
					if (hm.containsKey(or))
						hm.put(or, hm.get(or)+1);
					else
						hm.put(or, 1);
				}
		List <Map.Entry<OfficerRank, Integer>> ls = new ArrayList<Map.Entry<OfficerRank, Integer>>(hm.entrySet());
		Collections.sort(ls, new Comparator <Map.Entry<OfficerRank,Integer>>(){
			public int compare(Map.Entry<OfficerRank,Integer> me1, Map.Entry<OfficerRank,Integer> me2){
				int i = me1.getValue().compareTo(me2.getValue());
				if (i != 0)
					return -i;
				return me1.getKey().toString().compareTo(me2.getKey().toString());
            }
		} );
		return ls;
	}

}
