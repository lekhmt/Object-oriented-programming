package gui;

import figures.Figure;
import figures.Point;
import figures.Rectangle;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

public class CreateRectangle extends CreateFigure {

    CreateRectangle.MyFrame frame;
    Rectangle h;
    JButton button;

    public CreateRectangle(List<Figure> figures, List<Figure> figuresBuffer, int BUFFER_SIZE, Graphics g, App.MyPanel canvas) {
        this.figures = figures;
        this.figuresBuffer = figuresBuffer;
        this.BUFFER_SIZE = BUFFER_SIZE;
        this.g = g;
        this.canvas = canvas;
    }

    void getData(List<Figure> figuresBuffer, int BUFFER_SIZE, Graphics g, App.MyPanel canvas, String fileName) {
        frame = new CreateRectangle.MyFrame("Создать прямоугольник");
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        JLabel xCoordinate = new JLabel("Абсцисса левой верхней вершины: ");
        JTextField xCoordinateField = new JTextField(8);

        JLabel yCoordinate = new JLabel("Ордината левой верхней вершины: ");
        JTextField yCoordinateField = new JTextField(8);

        JLabel width = new JLabel("Длина: ");
        JTextField widthField = new JTextField(8);

        JLabel height = new JLabel("Ширина: ");
        JTextField heightField = new JTextField(8);

        JLabel theta = new JLabel("Угол поворота: ");
        JTextField thetaField = new JTextField(8);

        button = new JButton("OK");
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String xCoordinateText = xCoordinateField.getText();
                String yCoordinateText = yCoordinateField.getText();
                String widthFieldText = widthField.getText();
                String heightFieldText = heightField.getText();
                String thetaText = thetaField.getText();
                if (xCoordinateText.equals("") || yCoordinateText.equals("") || widthFieldText.equals("")
                        || heightFieldText.equals("") || thetaText.equals("")) {
                    JOptionPane.showMessageDialog(frame, "Заполните все поля");
                    return;
                }
                String template = "((-|\\+)?[0-9]+(\\.[0-9]+)?)+";
                if (!xCoordinateText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённая абсцисса не является числом");
                    return;
                }
                if (!yCoordinateText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённая ордината не является числом");
                    return;
                }
                if (!widthFieldText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённая длина не является числом");
                    return;
                }
                if (!heightFieldText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённая ширина не является числом");
                    return;
                }
                if (!thetaText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённый угол поворота не является числом");
                    return;
                }
                h = new Rectangle(new Point(Double.parseDouble(xCoordinateText), Double.parseDouble(yCoordinateText)),
                        Double.parseDouble(widthFieldText), Double.parseDouble(heightFieldText), Double.parseDouble(thetaText));
                figuresBuffer.add(h);
                if (figuresBuffer.size() >= BUFFER_SIZE){
                    controller();
                }
                frame.setVisible(false);
            }
        });

        JPanel contents = new JPanel(new FlowLayout(FlowLayout.LEFT));
        contents.add(xCoordinate);
        contents.add(xCoordinateField);
        contents.add(yCoordinate);
        contents.add(yCoordinateField);
        contents.add(width);
        contents.add(widthField);
        contents.add(height);
        contents.add(heightField);
        contents.add(theta);
        contents.add(thetaField);
        contents.add(button);

        frame.setContentPane(contents);
        frame.pack();
        frame.setVisible(true);
    }

    class MyFrame extends JFrame {
        public MyFrame(String title) {
            super(title);
        }
    }

}