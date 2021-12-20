package gui;

import figures.Figure;
import figures.Point;
import figures.Trapezoid;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

public class CreateTrapezoid extends CreateFigure{

    MyFrame frame;
    Trapezoid t;
    JButton button;

    public CreateTrapezoid(List<Figure> figures, List<Figure> figuresBuffer, int BUFFER_SIZE, Graphics g, App.MyPanel canvas) {
        this.figures = figures;
        this.figuresBuffer = figuresBuffer;
        this.BUFFER_SIZE = BUFFER_SIZE;
        this.g = g;
        this.canvas = canvas;
    }

    void getData() {
        frame = new CreateTrapezoid.MyFrame("Создать трапецию");
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        JLabel xCoordinate = new JLabel("Абсцисса центра: ");
        JTextField xCoordinateField = new JTextField(8);

        JLabel yCoordinate = new JLabel("Ордината центра: ");
        JTextField yCoordinateField = new JTextField(8);

        JLabel up_side = new JLabel("Длина верхней стороны: ");
        JTextField up_sideField = new JTextField(8);

        JLabel down_side = new JLabel("Длина нижней стороны: ");
        JTextField down_sideField = new JTextField(8);

        JLabel height = new JLabel("Высота: ");
        JTextField heightField = new JTextField(8);

        JLabel theta = new JLabel("Угол поворота: ");
        JTextField thetaField = new JTextField(8);

        button = new JButton("OK");
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String xCoordinateText = xCoordinateField.getText();
                String yCoordinateText = yCoordinateField.getText();
                String up_sideFieldText = up_sideField.getText();
                String down_sideFieldText = down_sideField.getText();
                String heightFieldText = heightField.getText();
                String thetaText = thetaField.getText();
                if (xCoordinateText.equals("") || yCoordinateText.equals("") || up_sideFieldText.equals("")
                        || down_sideFieldText.equals("") || heightFieldText.equals("") ||thetaText.equals("")) {
                    JOptionPane.showMessageDialog(frame, "Заполните все поля");
                    return;
                }
                String template = "((-|\\+)?[0-9]+(\\.[0-9]+)?)+";
                if (!xCoordinateText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённая абсцисса центра не является числом");
                    return;
                }
                if (!yCoordinateText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённая ордината центра не является числом");
                    return;
                }
                if (!up_sideFieldText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённая длина верхней стороны не является числом");
                    return;
                }
                if (!down_sideFieldText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённая длина нижней стороны не является числом");
                    return;
                }
                if (!heightFieldText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённая высота не является числом");
                    return;
                }
                if (!thetaText.matches(template)) {
                    JOptionPane.showMessageDialog(frame, "Ошибка: введённый угол поворота не является числом");
                    return;
                }
                t = new Trapezoid(new Point(Double.parseDouble(xCoordinateText), Double.parseDouble(yCoordinateText)),
                        Double.parseDouble(up_sideFieldText), Double.parseDouble(down_sideFieldText), Double.parseDouble(heightFieldText), Double.parseDouble(thetaText));
                figuresBuffer.add(t);
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
        contents.add(up_side);
        contents.add(up_sideField);
        contents.add(down_side);
        contents.add(down_sideField);
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