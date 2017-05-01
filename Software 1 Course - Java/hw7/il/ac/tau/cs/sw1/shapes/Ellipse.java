package il.ac.tau.cs.sw1.shapes;

public class Ellipse implements Shape{
	private int x, y, semiMajorAxis, semiMinorAxis;
	
	public Ellipse(int x, int y, int semiMajorAxis, int semiMinorAxis){
		this.x=x;
		this.y=y;
		this.semiMajorAxis=semiMajorAxis;
		this.semiMinorAxis=semiMinorAxis;
	}
	
	public int getX() {
		return x;
	}

	public void setX(int x) {
		this.x = x;
	}

	public int getY() {
		return y;
	}

	public void setY(int y) {
		this.y = y;
	}

	public int getSemiMajorAxis() {
		return semiMajorAxis;
	}

	public void setSemiMajorAxis(int semiMajorAxis) {
		this.semiMajorAxis = semiMajorAxis;
	}

	public int getSemiMinorAxis() {
		return semiMinorAxis;
	}

	public void setSemiMinorAxis(int semiMinorAxis) {
		this.semiMinorAxis = semiMinorAxis;
	}

	@Override
	public float getArea() {
		return (float)Math.PI*semiMajorAxis*semiMinorAxis;
	}

	@Override
	public float getPerimeter() {
		return (float)Math.PI*(semiMajorAxis+semiMinorAxis);
	}

	@Override
	public String getDetails() {
		return "Ellipse: X="+x+", Y="+y+", SemiMajorAxis="+semiMajorAxis+", SemiMinorAxis="+semiMinorAxis;
	}
}
