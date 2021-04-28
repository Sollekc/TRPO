=begin
Разработайте классы PointDecart (точка в декартовых координатах) и
PointPolar (точка в полярных координатах), которые содержат в себе следующую
информацию: поля — координаты точки, методы — конструктор, методы чтения координат,
методы установки значений координат, метод вывода значений координат на консоль.
Напишите адаптер ОБЪЕКТА, который приспособит PointPolar к PontDecart.
Продемонстрируйте работу паттерна.

=end

class PointDecart

    attr_accessor :x_coordinate, :y_coordinate

    def initialize(x_coordinate,y_coordinate)
        @x_coordinate = x_coordinate
        @y_coordinate = y_coordinate
    end

    def print()
           puts "x: #{@x_coordinate} \ny: #{@y_coordinate}"
    end
end

class PointPolar

    attr_accessor :radius, :phi

    def initialize(radius, phi)
        @radius = radius
        @phi = phi
    end

    def print()
           puts "radius: #{@radius} \nphi: #{@phi}"
    end
end

class Adapter 

    attr_accessor :pointPolar

    def initialize(pointPolar)
           @pointPolar = pointPolar
    end

    def transformation()
           return PointDecart.new(@pointPolar.radius * Math.cos(@pointPolar.phi),
                                  @pointPolar.radius * Math.sin(@pointPolar.phi))
    end
end


pointDecart = PointDecart.new(1, 2)
pointDecart.print
pointPolar = PointPolar.new(14.142135623730951, 0.7853981633974483)
pointPolar.print

Adapter.new(pointPolar).transformation.print
