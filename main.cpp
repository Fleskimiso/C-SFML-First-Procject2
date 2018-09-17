#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
void Vertex_Array_Update(sf::Vector2f rect_size,sf::VertexArray & Vertical_Array, sf::VertexArray & Horizontal_Array,sf::Vector2f Vertex_Difference);
sf::Vector2u Screen(1080,720);

sf::Vector2f Scale(1,1);

class Player //to do player class
{
private:
    sf::ConvexShape Player_shape;
    sf::Clock Speed_clock;
    sf::Vector2f Speed;
    sf::Clock Position_update_clock;
    sf::Clock Gravity_clock;

    float Gravity_Force;
public:
    bool set_Player_Position;
    Player(sf::Vector2f Position);
    sf::ConvexShape get_Player(); //this is useless funtion at least
    void move_player(sf::View& unupdated_view);
    void update_speed(sf::Keyboard::Key  Direction);
    void set_Player_Positionn(sf::Vector2f PP_pos);// this function is for debugging
    void reset_speed(bool Y_collision=false, bool X_collision=false);
    void gravity(); //restest speed
};



enum Colors {Red,Green,Blue};

int main()
{
    //view and window
    sf::RenderWindow window(sf::VideoMode(Screen.x, Screen.y),"Game engine",sf::Style::Resize | sf::Style::Default);

    sf::Vector2f Current_view_center(window.getView().getCenter());

    sf::View Window_view;
    Window_view = window.getView();

    sf::Vector2f Vertex_Difference(0,0);
// end of view and window

    //Rectangle managment
    sf::Vector2f Rect_Size(40,40);
    sf::RectangleShape On_cursor_rect;
    On_cursor_rect.setSize(Rect_Size);  //Follows cursor
    On_cursor_rect.setFillColor(sf::Color(255,255,255));
    On_cursor_rect.setOrigin(Rect_Size.x/2,Rect_Size.y/2);
    Colors On_cursor_rect_color = Green;
    //end of rectangle managment
    //vector of rectangles placed in map
    std::vector<sf::RectangleShape> Shapes;
     /// to do container std::map<
    //textures
    std::vector<sf::Texture> Texture_vector(2);
    for(int i=0; i<2; i++)
    {
        // Texture_vector.push_back(sf::Texture());
        switch(i)
        {
        case 0:
        {

            if(!Texture_vector.at(i).loadFromFile("grund.png"))
                std::cout << "An error has occured while loading grund tile" << std::endl;
            break;
        }
        case 1:
        {

            if(!Texture_vector.at(i).loadFromFile("water tile.png"))
                std::cout << "An error has occured while loading water tile" << std::endl;
            break;
        }
        }
    }
    On_cursor_rect.setTexture(&Texture_vector.at(0));
    int On_cursor_texture_vector_pointer =0;
    //to be done
    sf::VertexArray Vertical_VexArr; //Vertical Vertex array and it's initialization
    for(int i=0; i<Screen.x/Rect_Size.x; i++)
    {
        Vertical_VexArr.append(sf::Vertex(sf::Vector2f(i*Rect_Size.x,0),sf::Color::Blue));
        Vertical_VexArr.append(sf::Vertex(sf::Vector2f(i*Rect_Size.x,Screen.y),sf::Color::Green));
    }
    Vertical_VexArr.setPrimitiveType(sf::PrimitiveType::Lines);
    sf::VertexArray Horizontal_VexArr;
    for(int i=0; i<Screen.y/Rect_Size.y; i++)
    {
        Horizontal_VexArr.append(sf::Vertex(sf::Vector2f(0,i*Rect_Size.y),sf::Color::Red));
        Horizontal_VexArr.append(sf::Vertex(sf::Vector2f(Screen.x,i*Rect_Size.y),sf::Color::Green));
    }
    Horizontal_VexArr.setPrimitiveType(sf::PrimitiveType::Lines);
    // end of initialization


    //player pointer meant or maybe no let's do normal

    Player Player_unit(sf::Vector2f(0,0));
    //end
    bool buliding_a_game = true;


    while (window.isOpen())
    {
        sf::Event event;
        if(buliding_a_game)
        {
            while (window.pollEvent(event))
            {
                switch(event.type)
                {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed:
                {
                    if(Player_unit.set_Player_Position == false)
                    {

                        if(event.mouseButton.button == sf::Mouse::Left) //Positioning the Rectangles
                        {
                            sf::Vector2f Cursor_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                            sf::Vector2f modulo(int( Cursor_position.x)%int(Rect_Size.x),int (Cursor_position.y)%int(Rect_Size.y));
                            if(modulo.x <0)
                            {
                                modulo.x = -(Rect_Size.x + modulo.x)+Rect_Size.x/2;
                            }
                            else
                            {
                                modulo.x = -(modulo.x) + Rect_Size.x/2;
                            }
                            if(modulo.y < 0)
                            {
                                modulo.y = -(Rect_Size.y + modulo.y) + Rect_Size.y/2;
                            }
                            else
                            {
                                modulo.y = -(modulo.y) + Rect_Size.y/2;
                            }
                            On_cursor_rect.setPosition(Cursor_position + modulo );
                            Shapes.push_back(On_cursor_rect);

                        }
                    }
                    else if(Player_unit.set_Player_Position == true)
                    {
                        if(event.mouseButton.button == sf::Mouse::Left)
                        {
                            Player_unit.set_Player_Position = false;

                        }
                    }
                    break;
                }
                case sf::Event::Resized:
                {
                    Window_view.setSize(event.size.width,event.size.height);
                    int X_1 = event.size.width;
                    int X_2 = Screen.x;
                    int Y_1 = event.size.height, Y_2 = Screen.y;
                    Window_view.setCenter(Window_view.getCenter()+sf::Vector2f((X_1-X_2)/2, (Y_1-Y_2)/2 ));
                    std::cout << "vent size x - screen .x :   " << X_1-X_2 << std::endl;
                    //Updating view when Screen is resized
                    //sf::FloatRect visibleArea(0,0, event.size.width, event.size.height);
                    Screen.y = event.size.height; //Problem solved by fixing window view;
                    Screen.x = event.size.width;
                    std::cout << "X: " << Screen.x << " Y: " << Screen.y << std::endl;
                    Vertex_Array_Update(Rect_Size,Vertical_VexArr,Horizontal_VexArr,Vertex_Difference);
                    window.setView(Window_view);
                    //Window view fixed by adjusting the view size and moveing it proprely sp that it covers all Vertices
                    //now let's do the vertex arrays update;
                    break;
                }
                case sf::Event::KeyPressed: //Configures view not done in 100%
                {
                    if(event.key.code == sf::Keyboard::W)
                    {
                        Window_view.move(0,-Rect_Size.y);
                        window.setView(Window_view);
                        Vertex_Difference.y-=Rect_Size.y;
                    }
                    else if(event.key.code == sf::Keyboard::S)
                    {
                        Window_view.move(0,Rect_Size.y);
                        window.setView(Window_view);
                        Vertex_Difference.y+=Rect_Size.y;
                    }
                    else if(event.key.code == sf::Keyboard::A)
                    {
                        Window_view.move(-Rect_Size.x,0);
                        window.setView(Window_view);
                        Vertex_Difference.x-=Rect_Size.x;
                    }
                    else if(event.key.code == sf::Keyboard::D)
                    {
                        Window_view.move(Rect_Size.x,0);
                        window.setView(Window_view);
                        Vertex_Difference.x+=Rect_Size.x;
                    }
                    else if(event.key.code == sf::Keyboard::Num1)
                    {
                        On_cursor_rect_color = Red;
                    }
                    else if(event.key.code == sf::Keyboard::Num2)
                    {
                        On_cursor_rect_color = Green;
                    }
                    else if(event.key.code == sf::Keyboard::Num3)
                    {
                        On_cursor_rect_color = Blue;
                    }
                    else if(event.key.code == sf::Keyboard::Escape)
                    {
                        buliding_a_game = false;
                        window.setKeyRepeatEnabled(false);
                    }
                    else if(event.key.code == sf::Keyboard::Left)
                    {
                        if(On_cursor_texture_vector_pointer == 0)
                        {
                            On_cursor_texture_vector_pointer = Texture_vector.size() -1;
                            On_cursor_rect.setTexture(&Texture_vector.at(On_cursor_texture_vector_pointer));
                        }
                        else
                        {
                            On_cursor_texture_vector_pointer-=1;
                            On_cursor_rect.setTexture(&Texture_vector.at(On_cursor_texture_vector_pointer));
                        }
                    }
                    else if(event.key.code == sf::Keyboard::Right)
                    {
                        if(On_cursor_texture_vector_pointer == Texture_vector.size()-1)
                        {
                            On_cursor_texture_vector_pointer = 0;
                            On_cursor_rect.setTexture(&Texture_vector.at(On_cursor_texture_vector_pointer));
                        }
                        else
                        {
                            On_cursor_texture_vector_pointer+=1;
                            On_cursor_rect.setTexture(&Texture_vector.at(On_cursor_texture_vector_pointer));
                        }
                    }
                    else if(event.key.code == sf::Keyboard::P)
                    {
                        Player_unit.set_Player_Position = true;
                    }
                    else if(event.key.code == sf::Keyboard::Up)
                    {
                        Window_view.zoom(1.5);
                        window.setView(Window_view);
                    }
                    else if(event.key.code == sf::Keyboard::Down)
                    {
                        Window_view.zoom((2.0/3.0));
                        window.setView(Window_view);
                    }
                    Vertex_Array_Update(Rect_Size,Vertical_VexArr,Horizontal_VexArr,Vertex_Difference);

                    break;
                }

                case sf::Event::MouseWheelScrolled:
                {

                    if(On_cursor_rect_color == Red)
                    {
                        sf::Color colorr = On_cursor_rect.getFillColor();
                        if(static_cast<int>(colorr.r) <=3 && event.mouseWheelScroll.delta <0 ||
                                static_cast<int>(colorr.r) >=253 && event.mouseWheelScroll.delta > 0 )
                            break;
                        colorr.r+=event.mouseWheelScroll.delta*3;
                        On_cursor_rect.setFillColor(colorr);


                        std::cout << "whell:  " << event.mouseWheel.delta << "   " << event.mouseWheelScroll.delta << " Color: " << int(colorr.r) << std::endl;
                    }
                    else if(On_cursor_rect_color == Green)
                    {
                        sf::Color colorr = On_cursor_rect.getFillColor();
                        if(static_cast<int>(colorr.g) <=3 && event.mouseWheelScroll.delta <0 ||
                                static_cast<int>(colorr.g) >=253 && event.mouseWheelScroll.delta > 0 )
                            break;
                        colorr.g+=event.mouseWheelScroll.delta*3;
                        On_cursor_rect.setFillColor(colorr);
                        std::cout << event.mouseWheelScroll.delta << " Color: " << int(colorr.g) << std::endl;
                    }
                    else if(On_cursor_rect_color == Blue)
                    {
                        sf::Color colorr = On_cursor_rect.getFillColor();
                        if(static_cast<int>(colorr.b) <=3 && event.mouseWheelScroll.delta <0 ||
                                static_cast<int>(colorr.b) >=253 && event.mouseWheelScroll.delta > 0 )
                            break;
                        colorr.b+=event.mouseWheelScroll.delta*3;
                        On_cursor_rect.setFillColor(colorr);
                        std::cout << event.mouseWheelScroll.delta << " Color: " << int(colorr.b) << std::endl;
                    }
                    break;
                }
                default:
                    break;

                }
            }
            if(Player_unit.set_Player_Position == false)
                On_cursor_rect.setPosition(sf::Vector2f (window.mapPixelToCoords(sf::Mouse::getPosition(window))));
            else if(Player_unit.set_Player_Position == true)
                Player_unit.set_Player_Positionn(sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window))));
            // get the current mouse position in the window
//sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

// convert it to world coordinates
//sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            window.clear();
            for(int i=0; i <Shapes.size(); i++)
            {
                window.draw(Shapes.at(i));
            }
            window.draw(On_cursor_rect);
            window.draw(Vertical_VexArr);
            window.draw(Player_unit.get_Player());
            window.draw(Horizontal_VexArr);
        }
        else
        {
            while(window.pollEvent(event))
            {
                switch(event.type)
                {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::KeyPressed:
                {
                    switch(event.key.code)
                        {
                        case sf::Keyboard::P:
                            {
                                Player_unit.set_Player_Positionn(window.mapPixelToCoords(sf::Mouse::getPosition(window)));//function for debugging
                                break;
                            }
                        default:
                            {
                                break;
                            }
                        }
                    break;
                }
                case sf::Event::KeyReleased:
                    { //to do x cord in speed vecor set to 0 no
                        Player_unit.reset_speed(false,true);
                        break;
                    }
                case sf::Event::Resized:
                {
                    //to do in free time
                }
                default:
                    break;
                }
            }//There won't be any accelration speed will be constant only gravity force makes acceleration
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                Player_unit.update_speed(sf::Keyboard::A);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                Player_unit.update_speed(sf::Keyboard::D);
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                Player_unit.update_speed(sf::Keyboard::S);
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                Player_unit.update_speed(sf::Keyboard::W);
            }

            Player_unit.gravity();
            Player_unit.move_player(Window_view);
            window.setView(Window_view);
            window.clear();
           // Vertex_Difference -= Window_view.getCenter() - Player_unit.get_Player().getPosition();
            //Window_view.setCenter(Player_unit.get_Player().getPosition());
            Vertex_Array_Update(Rect_Size,Vertical_VexArr,Horizontal_VexArr,Vertex_Difference);
            for(int i=0; i<Shapes.size(); i++)
            {
                window.draw(Shapes.at(i));
            }
            window.draw(Vertical_VexArr);
            window.draw(Player_unit.get_Player());
            window.draw(Horizontal_VexArr);
        }



        window.display();
    }

    return 0;
}
void Vertex_Array_Update(sf::Vector2f Rect_Size, sf::VertexArray& Vertical_Array, sf::VertexArray& Horizontal_Array,sf::Vector2f Vertex_Difference)
{
    Vertical_Array.clear();
    Horizontal_Array.clear();
    for(int i=0; i<Screen.x/Rect_Size.x; i++)
    {
        Vertical_Array.append(sf::Vertex(sf::Vector2f(i*Rect_Size.x+Vertex_Difference.x,0+Vertex_Difference.y),sf::Color::Blue));
        Vertical_Array.append(sf::Vertex(sf::Vector2f(i*Rect_Size.x+Vertex_Difference.x,Screen.y+Vertex_Difference.y),sf::Color::Green));
    }
    for(int i=0; i<Screen.y/Rect_Size.y; i++)
    {
        Horizontal_Array.append(sf::Vertex(sf::Vector2f(0+Vertex_Difference.x,i*Rect_Size.y+Vertex_Difference.y),sf::Color::Red));
        Horizontal_Array.append(sf::Vertex(sf::Vector2f(Screen.x+Vertex_Difference.x,i*Rect_Size.y+Vertex_Difference.y),sf::Color::Green));
    }
}
Player::Player(sf::Vector2f Position)
{
    this->Player_shape.setPointCount(5);
    Player_shape.setPoint(0,sf::Vector2f(0,15));
    Player_shape.setPoint(1,sf::Vector2f(20,0));
    Player_shape.setPoint(2,sf::Vector2f(40,15));
    Player_shape.setPoint(3,sf::Vector2f(30,40));
    Player_shape.setPoint(4,sf::Vector2f(10,40));
    Player_shape.setFillColor(sf::Color::Blue);
    Player_shape.setOutlineColor(sf::Color::Red);
    Player_shape.setOutlineThickness(-2);

    Player_shape.setPosition(Position);
    Player_shape.setOrigin(Player_shape.getGlobalBounds().width/2,Player_shape.getGlobalBounds().height/2);
    set_Player_Position = false;
    Speed = sf::Vector2f(100,0);
    Gravity_Force = 100;
}
sf::ConvexShape Player::get_Player()
{
    return this->Player_shape;
}
void Player::set_Player_Positionn(sf::Vector2f PP_pos)
{
    Player_shape.setPosition(PP_pos);
}
void Player::move_player(sf::View& unupdated_view)
{
    if(Position_update_clock.getElapsedTime().asSeconds() > 1)
    {
        Position_update_clock.restart();
        this->Player_shape.move(this->Speed*Position_update_clock.restart().asSeconds());
    }
    else
    {
        if(this->Player_shape.getPosition().y + 50 > Screen.y )
        {
            Speed.y = 0;
        }
        sf::Vector2f move_difference = Speed*Position_update_clock.restart().asSeconds();
        this->Player_shape.move(move_difference);
        unupdated_view.move(move_difference);
        std::cout << "X: " << Player_shape.getPosition().x  << "Y: " << Player_shape.getPosition().y  << std::endl;
    }

}
void Player::update_speed(sf::Keyboard::Key Direction)
{
    switch(Direction)
    {
        if(this->Speed_clock.getElapsedTime().asMilliseconds() > 10)
         {
            Speed_clock.restart();
         }
    case sf::Keyboard::A:
    {
        if(Speed.x>=-200)
        {
            Speed.x = -200;
        }
        break;
    }
    case sf::Keyboard::D:
    {
        if(Speed.x <= 200)
        {
            Speed.x = 200;
        }
        break;
    }
    case sf::Keyboard::W:
    {
        if(! (Speed.y <= -5) )
        {
            Speed.y = -200;
        }
        break;
    }
    case sf::Keyboard::S:
    {
        if(Speed.y <= 200)
        {
            Speed.y = 200;
        }
        break;
    }
    default:
        break;
    }
}
void Player::reset_speed(bool Y_collision, bool X_collision)
{
    if(Y_collision==true)
    {
        Speed.y=0;
    }
    if(X_collision == true)
    {
      Speed.x =0;
    }
}
void Player::gravity()
{
    if(Gravity_clock.getElapsedTime().asMilliseconds() > 10)
    {
        Gravity_clock.restart();
        Speed.y+=Gravity_Force;
    }
    else
    {
        Speed.y+= Gravity_Force *Gravity_clock.restart().asSeconds();
    }
}
