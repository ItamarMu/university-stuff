package il.ac.tau.cs.sw1.ex9.starfleet;

import java.util.Set;

public class TransportShip extends AbstSpaceship{

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = super.hashCode();
		result = prime * result + cargoC;
		result = prime * result + passengerC;
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
		TransportShip other = (TransportShip) obj;
		if (cargoC != other.cargoC)
			return false;
		if (passengerC != other.passengerC)
			return false;
		return true;
	}

	private int cargoC;
	private int passengerC;
	
	public TransportShip(String name, int CoYear, float MaxSpeed, Set<CrewMember> members,
			int cargoC, int passengerC) {
		super(name, CoYear, MaxSpeed, members);
		this.cargoC = cargoC;
		this.passengerC = passengerC;
		this.AMC = 3000;
	}
	
	public int getAnnualMaintenanceCost(){
		return super.getAnnualMaintenanceCost()+(5*cargoC)+(3*passengerC);
	}

	public String toString(){
		String sp = System.lineSeparator();
		return super.toString() + 
				sp+"\t"+"CargoCapacity="+cargoC+
				sp+"\t"+"PassengerCapacity="+passengerC;
	}
}
