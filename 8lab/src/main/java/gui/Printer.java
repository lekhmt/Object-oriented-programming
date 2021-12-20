package gui;

import figures.Figure;

import java.awt.*;
import java.util.List;

public class Printer extends Thread {

    List<Figure> figuresBuffer;
    Graphics g;
    App.MyPanel canvas;

    Printer(List<Figure> figuresBuffer, Graphics g, App.MyPanel canvas) {
        this.figuresBuffer = figuresBuffer;
        this.g = g;
        this.canvas = canvas;
    }

    public void run(){
        try {
            Thread.sleep(5000);
        } catch (InterruptedException ex){
            ex.printStackTrace();
        }
        g.setColor(Color.black);
        for (var f : figuresBuffer) {
            f.draw(g);
        }
        canvas.repaint();
        figuresBuffer.clear();
    }

}
