package il.ac.tau.cs.sw1.shapes;

public class Circle implements Shape{
	private int x, y, radius;
	
	public Circle (int x, int y, int radius){
		this.x=x;
		this.y=y;
		this.radius=radius;
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

	public int getRadius() {
		return radius;
	}

	public void setRadius(int radius) {
		this.radius = radius;
	}

	@Override
	public float getArea() {
		return (float)Math.PI*radius*radius;
	}

	@Override
	public float getPerimeter() {
		return (float)Math.PI*radius*2;
	}

	@Override
	public String getDetails() {
		return "Circle: X="+x+", Y="+y+", Radius="+radius;
	}
}
