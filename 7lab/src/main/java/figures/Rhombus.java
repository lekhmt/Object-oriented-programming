package figures;

import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Line2D;

public class Rhombus extends Figure {

    private final double firstDiagonal;
    private final double secondDiagonal;

    public Rhombus(Point center, double firstDiagonal, double secondDiagonal, double theta) {
        this.center = center;
        this.firstDiagonal = firstDiagonal;
        this.secondDiagonal = secondDiagonal;
        this.theta = theta;
    }

    public double getFirstDiagonal() {
        return firstDiagonal;
    }

    public double getSecondDiagonal() {
        return secondDiagonal;
    }

    @Override
    public String toString() {
        return "Rhombus{" +
                "center=" + center +
                ", firstDiagonal=" + firstDiagonal +
                ", secondDiagonal=" + secondDiagonal +
                '}';
    }

    public void draw(Graphics g) {
        Graphics2D g2d = (Graphics2D) g;
        Shape l1 = new Line2D.Double(center.getX() - firstDiagonal / 2, center.getY(),
                center.getX(), center.getY() + secondDiagonal / 2);
        Shape l2 = new Line2D.Double(center.getX(), center.getY() + secondDiagonal / 2,
                center.getX() + firstDiagonal / 2, center.getY());
        Shape l3 = new Line2D.Double(center.getX() + firstDiagonal / 2, center.getY(),
                center.getX(), center.getY() - secondDiagonal / 2);
        Shape l4 = new Line2D.Double(center.getX(), center.getY() - secondDiagonal / 2,
                center.getX() - firstDiagonal / 2, center.getY());
        AffineTransform old = g2d.getTransform();
        g2d.rotate(Math.toRadians(theta), center.getX(), center.getY());
        g2d.draw(l1);
        g2d.draw(l2);
        g2d.draw(l3);
        g2d.draw(l4);
        g2d.setTransform(old);
    }

}
