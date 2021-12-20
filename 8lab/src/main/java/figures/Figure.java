package figures;

import java.awt.*;

public abstract class Figure {

    protected Point center;
    protected double theta;

    public Point getCenter() {
        return center;
    }

    public double getTheta() {
        return theta;
    }

    public abstract void draw(Graphics g);

}