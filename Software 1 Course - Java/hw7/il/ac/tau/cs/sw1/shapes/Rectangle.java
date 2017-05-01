package il.ac.tau.cs.sw1.shapes;

public class Rectangle implements Shape{
	private int x, y, width, height;
	
	public Rectangle (int x, int y, int width, int height){
		this.x=x;
		this.y=y;
		this.width=width;
		this.height=height;
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

	public int getWidth() {
		return width;
	}

	public void setWidth(int width) {
		this.width = width;
	}

	public int getHeight() {
		return height;
	}

	public void setHeight(int height) {
		this.height = height;
	}

	@Override
	public float getArea() {
		return width*height;
	}

	@Override
	public float getPerimeter() {
		return 2*(width+height);
	}

	@Override
	public String getDetails() {
		return "Rectangle: X="+x+", Y="+y+", Width="+width+", Height="+height;
	}
	
}
