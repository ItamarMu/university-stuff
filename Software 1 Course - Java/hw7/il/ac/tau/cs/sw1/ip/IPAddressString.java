package il.ac.tau.cs.sw1.ip;

public class IPAddressString implements IPAddress {
	
	private String address;
	
	IPAddressString(String address) {
		this.address=address;
	}

	@Override
	public String toString() {
		return address;
	}

	@Override
	public boolean equals(IPAddress other) {
		String [] arr = address.split("\\.");
		for (int i=0; i<4; i++)
			if (Integer.parseInt(arr[i])!=other.getOctet(i))
				return false;
		return true;
	}

	@Override
	public int getOctet(int index) {
		String [] arr = address.split("\\.");
		return Integer.parseInt(arr[index]);
	}

	@Override
	public boolean isPrivateNetwork(){
		String [] arr = address.split("\\.");
		if (Integer.parseInt(arr[0])==10)
			return true;
		if (Integer.parseInt(arr[0])==172)
			if (Integer.parseInt(arr[1])>=16 && Integer.parseInt(arr[1])<=31)
				return true;
		if (Integer.parseInt(arr[0])==192 && Integer.parseInt(arr[1])==168)
			return true;
		if (Integer.parseInt(arr[0])==169 && Integer.parseInt(arr[1])==254)
			return true;
		return false;
	}
	
}
