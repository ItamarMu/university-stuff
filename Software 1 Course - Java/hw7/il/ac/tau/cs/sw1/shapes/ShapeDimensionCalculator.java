package il.ac.tau.cs.sw1.shapes;


import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;


public class ShapeDimensionCalculator {




	public static void main(String[] args) throws IOException {

		Shape[] shapes = getShapesFromUser();
		writeShapesToFile("shapes_output.txt", shapes);
	
	}

	public static Shape[] getShapesFromUser() {
		String sep = System.lineSeparator();
		System.out.println("Shape Dimension Calculator"+sep);
		Scanner user = new Scanner(System.in);
		Shape[] shapes = new Shape[20];
		boolean sgo=true, dgo=true;// sgo-if "X" typed, dgo-if default case->won't print
		String typ="";
		int x, y, cnt=0;
		while (sgo && cnt<20){
			System.out.println("Please choose shape type:"+sep+"E – Ellipse"+sep+"R – Rectangle"+
								sep+"C – Circle"+sep+"X - Exit");
			typ=user.nextLine();
			switch (typ){
			case "E":
				System.out.print("Please enter X coordinate: ");
				x=Integer.parseInt(user.nextLine());
				System.out.print("Please enter Y coordinate: ");
				y=Integer.parseInt(user.nextLine());
				System.out.print("Please enter semi-major axis length: ");
				int SMAA=Integer.parseInt(user.nextLine());
				System.out.print("Please enter semi-minor axis length: ");
				int SMIA=Integer.parseInt(user.nextLine());
				shapes[cnt]=new Ellipse(x, y, SMAA, SMIA);
				break;
			case "R":
				System.out.print("Please enter X coordinate: ");
				x=Integer.parseInt(user.nextLine());
				System.out.print("Please enter Y coordinate: ");
				y=Integer.parseInt(user.nextLine());
				System.out.print("Please enter width: ");
				int width=Integer.parseInt(user.nextLine());
				System.out.print("Please enter height: ");
				int height=Integer.parseInt(user.nextLine());
				shapes[cnt]=new Rectangle(x, y, width, height);
				break;
			case "C":
				System.out.print("Please enter X coordinate: ");
				x=Integer.parseInt(user.nextLine());
				System.out.print("Please enter Y coordinate: ");
				y=Integer.parseInt(user.nextLine());
				System.out.print("Please enter radius: ");
				int radius=Integer.parseInt(user.nextLine());
				shapes[cnt]=new Circle(x, y, radius);
				break;
			case "X":
				sgo=false;
				break;
				default:
					System.out.println("Unknown command. Please try again."+sep);
					dgo=false;
					break;
			}
			if (sgo && dgo)
				System.out.println("Shape added: ["+shapes[cnt++].getDetails()+"]"+sep);
			dgo=true;
		}
		user.close();
		return shapes;
	}

	public static void writeShapesToFile(String outputFilename, Shape[] shapes) throws IOException {
		File toFile = new File(outputFilename);
		BufferedWriter bw = new BufferedWriter(new FileWriter(toFile));
		String sep = System.lineSeparator();
		int CirNum=0, EllNum=0, RecNum=0;
		float totalArea=0, totalPerimeter=0;
		bw.write("Shape Dimension Calculator"+sep+sep);
		for (int i=0; i<shapes.length && shapes[i]!=null; i++){
			bw.write(shapes[i].getDetails()+sep+"Area: "+round2(shapes[i].getArea())+", Perimeter: "+
					round2(shapes[i].getPerimeter())+sep+sep);
			totalArea+=shapes[i].getArea();
			totalPerimeter+=shapes[i].getPerimeter();
			if (shapes[i] instanceof Rectangle)
				RecNum++;
			if (shapes[i] instanceof Circle)
				CirNum++;
			if (shapes[i] instanceof Ellipse)
				EllNum++;
		}
		bw.write("Total number of shapes: "+(CirNum+EllNum+RecNum)+sep+"Number of Circles: "+
		CirNum+sep+"Number of Ellipses: "+EllNum+sep+"Number of Rectangles: "+RecNum+sep+
					"Total Area sum: "+round2(totalArea)+sep+"Total Perimeter sum: "+round2(totalPerimeter));
		bw.close();
	}
	
	public static String round2 (float x){
		return String.format("%.2f", Math.ceil(x*100)/100);
	}
	
	

}
