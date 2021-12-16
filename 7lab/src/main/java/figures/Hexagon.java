package figures;

import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Line2D;

public class Hexagon extends AbstractPolygon{

    public Hexagon(Point center, double side, double theta){
        this.center = center;
        this.side = side;
        this.counter = 6;
        this.theta = theta;
    }

    @Override
    public String toString() {
        return "Hexagon{" +
                "side=" + side +
                ", center=" + center +
                '}';
    }

    private double x(int i){
        double R = side / (2 * Math.sin(Math.PI / 6));
        return center.getX() + R * Math.cos(Math.PI / 6 * (1 + 2 * i));
    }

    private double y(int i){
        double R = side / (2 * Math.sin(Math.PI / 6));
        return center.getY() + R * Math.sin(Math.PI / 6 * (1 + 2 * i));
    }

    public void draw(Graphics g) {
        Graphics2D g2d = (Graphics2D) g;
        Shape l1 = new Line2D.Double(x(0), y(0), x(1), y(1));
        Shape l2 = new Line2D.Double(x(1), y(1), x(2), y(2));
        Shape l3 = new Line2D.Double(x(2), y(2), x(3), y(3));
        Shape l4 = new Line2D.Double(x(3), y(3), x(4), y(4));
        Shape l5 = new Line2D.Double(x(4), y(4), x(5), y(5));
        Shape l6 = new Line2D.Double(x(5), y(5), x(0), y(0));
        AffineTransform old = g2d.getTransform();
        g2d.rotate(Math.toRadians(theta), center.getX(), center.getY());
        g2d.draw(l1);
        g2d.draw(l2);
        g2d.draw(l3);
        g2d.draw(l4);
        g2d.draw(l5);
        g2d.draw(l6);
        g2d.setTransform(old);
    }

}
