package gui;

import figures.Figure;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.List;

public class App {

    MyFrame frame;
    MyPanel canvas;
    String fileName;
    BufferedImage image;
    List<Figure> figures = new ArrayList<>();
    List<Figure> figuresBuffer = new ArrayList<>();
    final int BUFFER_SIZE = 2;

    public App() {
        frame = new MyFrame("Редактор фигур");
        frame.setSize(1800, 1000);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JMenuBar menuBar = new JMenuBar();
        frame.setJMenuBar(menuBar);
        menuBar.setBounds(0, 0, 400, 400);
        JMenu figureMenu = new JMenu("Фигуры");
        menuBar.add(figureMenu);

        Action addRhombusAction = new AbstractAction("Добавить ромб") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Graphics g = image.getGraphics();
                CreateRhombus cr = new CreateRhombus(figures, figuresBuffer, BUFFER_SIZE, g, canvas);
                cr.getData(figuresBuffer, BUFFER_SIZE, g, canvas, fileName);
            }
        };

        JMenuItem addRhombus = new JMenuItem(addRhombusAction);
        figureMenu.add(addRhombus);

        Action addPentagonAction = new AbstractAction("Добавить трапецию") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Graphics g = image.getGraphics();
                CreateTrapezoid cp = new CreateTrapezoid(figures, figuresBuffer, BUFFER_SIZE, g, canvas);
                cp.getData();
            }
        };

        JMenuItem addPentagon = new JMenuItem(addPentagonAction);
        figureMenu.add(addPentagon);

        Action addHexagonAction = new AbstractAction("Добавить прямоугольник") {
            @Override
            public void actionPerformed(ActionEvent e) {
                Graphics g = image.getGraphics();
                CreateRectangle ch = new CreateRectangle(figures, figuresBuffer, BUFFER_SIZE, g, canvas);
                ch.getData(figuresBuffer, BUFFER_SIZE, g, canvas, fileName);
            }
        };

        JMenuItem addHexagon = new JMenuItem(addHexagonAction);
        figureMenu.add(addHexagon);

        canvas = new MyPanel();
        canvas.setBounds(30, 30, 1740, 880);
        canvas.setBackground(Color.white);
        canvas.setOpaque(true);
        frame.add(canvas);


        frame.setLayout(null);
        frame.setVisible(true);

    }

    static class MyFrame extends JFrame {
        public void paint(Graphics g) {
            super.paint(g);
        }

        public MyFrame(String title) {
            super(title);
        }
    }

    class MyPanel extends JPanel {
        public MyPanel() {
        }

        public void paintComponent(Graphics g) {
            if (image == null) {
                image = new BufferedImage(this.getWidth(), this.getHeight(), BufferedImage.TYPE_INT_RGB);
                Graphics2D g2d = (Graphics2D) image.getGraphics();
                g2d.setColor(Color.white);
                g2d.fillRect(0, 0, this.getWidth(), this.getHeight());
            }
            super.paintComponent(g);
            g.drawImage(image, 0, 0, this);
        }
    }

}