package figures;

import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Line2D;

public class Rectangle extends Figure {

    private final double width;
    private final double height;
    public Rectangle(Point center, double width, double height, double theta){
        this.width = width;
        this.height = height;
        this.center = center;
        this.theta = theta;
    }

    public double getWidth() {
        return width;
    }

    public double getHeight() {
        return height;
    }

    public void draw(Graphics g) {
        Graphics2D g2d = (Graphics2D) g;
        Shape l1 = new Line2D.Double(center.getX(), center.getY(), center.getX() + width, center.getY());
        Shape l2 = new Line2D.Double(center.getX() + width, center.getY(), center.getX() + width, center.getY() + height);
        Shape l3 = new Line2D.Double(center.getX() + width, center.getY() + height, center.getX(), center.getY() + height);
        Shape l4 = new Line2D.Double(center.getX(), center.getY() + height, center.getX(), center.getY());

        AffineTransform old = g2d.getTransform();
        g2d.rotate(Math.toRadians(theta), center.getX(), center.getY());
        g2d.draw(l1);
        g2d.draw(l2);
        g2d.draw(l3);
        g2d.draw(l4);
        g2d.setTransform(old);
    }

}