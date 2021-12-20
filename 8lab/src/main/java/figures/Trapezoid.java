package figures;

import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Line2D;

public class Trapezoid extends Figure {

    private final double up_side;
    private final double down_side;
    private final double height;
    public Trapezoid(Point center, double up_side, double down_side, double height, double theta){
        this.up_side = up_side;
        this.down_side = down_side;
        this.height = height;
        this.center = center;
        this.theta = theta;
    }

    public double getUp_side() {
        return up_side;
    }

    public double getDown_side() {
        return down_side;
    }

    public double getHeight() {
        return height;
    }

    public void draw(Graphics g) {
        Graphics2D g2d = (Graphics2D) g;
        System.out.println(up_side);
        System.out.println(down_side);
        System.out.println(height);
        System.out.println(center);
        Shape l1 = new Line2D.Double(center.getX() - up_side / 2, center.getY() - height / 2, center.getX() + up_side / 2, center.getY() - height / 2);
        Shape l2 = new Line2D.Double(center.getX() + up_side / 2, center.getY() - height / 2, center.getX() + down_side / 2, center.getY() + height / 2);
        Shape l3 = new Line2D.Double(center.getX() + down_side / 2, center.getY() + height / 2, center.getX() - down_side / 2, center.getY() + height / 2);
        Shape l4 = new Line2D.Double(center.getX() - down_side / 2, center.getY() + height / 2, center.getX() - up_side / 2, center.getY() - height / 2);

        AffineTransform old = g2d.getTransform();
        g2d.rotate(Math.toRadians(theta), center.getX(), center.getY());
        g2d.draw(l1);
        g2d.draw(l2);
        g2d.draw(l3);
        g2d.draw(l4);
        g2d.setTransform(old);
    }
}