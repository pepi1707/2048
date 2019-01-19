#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstring>

const int MAXN = 6;
int game[MAXN][MAXN];
int prev[102][MAXN][MAXN];
int VER = 0;
int points[102];
int n = 4;
long long score;
int undo_counter = 0;

void preparation()
{
    int i,j;
    score = 0;
    for(i=0;i<100;i++)
        points[i]=0;
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            prev[0][i][j]=0;
            game[i][j]=0;
        }
    }
    int first_x = rand()%n + 1, first_y = rand()%n + 1, second_x = rand()%n + 1, second_y = rand()%n + 1;

    while(second_x == first_x && second_y == first_y)
    {
        second_x = rand()%n + 1;
        second_y = rand()%n + 1;
    }
    int chance = rand()%2;
    prev[0][first_x][first_y] = game[first_x][first_y] = 2;
    prev[0][second_x][second_y] = game[second_x][second_y] = 2 + chance*2;
}

void gen_new_block()
{
    int new_x = rand()%n + 1 ,new_y = rand()%n + 1;
    while(game[new_x][new_y]!=0)
    {
        new_x = rand()%n + 1;
        new_y = rand()%n + 1;
    }
    int chance = rand()%2;
    game[new_x][new_y] = 2 + chance*2;
}

void rotate_right()
{
    int i,j;
    int help_matrix[MAXN][MAXN];
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            help_matrix[j][n-i+1] = game[i][j];
        }
    }
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            game[i][j] = help_matrix[i][j];
        }
    }
}

void push_right()
{
    int help_matrix[MAXN][MAXN];
    int i,j;
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            help_matrix[i][j]=0;
        }
    }
    for(i=1;i<=n;i++)
    {
        int idx = n;
        for(j=n;j>=1;j--)
        {
            if(game[i][j]!=0)
            {
                help_matrix[i][idx] = game[i][j];
                idx--;
            }
        }
    }
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            game[i][j] = help_matrix[i][j];
        }
    }
}

void combine_right()
{
    int i,j;
    points[VER] = 0;
    for(i=1;i<=n;i++)
    {
        for(j=n;j>=2;j--)
        {
            if(game[i][j]==game[i][j-1])
            {
                game[i][j]*=2;
                points[VER]+=game[i][j];
                game[i][j-1]=0;
                j--;
            }
        }
    }
}

bool can_handle()
{
    int i,j;
    for(i=1;i<=n;i++)
    {
        for(j=1;j<n;j++)
        {
            if(game[i][j]!=0 && game[i][j] == game[i][j+1])
            {
                return true;
            }
            if(game[i][j] !=0 && game[i][j+1] == 0)
            {
                return true;
            }
        }
    }
    return false;
}
void update_version()
{
    int i,j;
    if(VER<100)
    {
        score += points[VER];
        VER++;
        for(i=1;i<=n;i++)
        {
            for(j=1;j<=n;j++)
            {
                prev[VER][i][j] = game[i][j];
            }
        }
        return;
    }
    int k;
    for(k=0;k<VER;k++)
    {
        for(i=1;i<=n;i++)
        {
            for(j=1;j<=n;j++)
            {
                points[k] = points[k + 1];
                prev[k][i][j] = prev[k+1][i][j];
            }
        }
    }
    points[VER] = 0;
    score += points[VER-1];
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            prev[VER][i][j] = game[i][j];
        }
    }
}

void handle_undo()
{
    if(VER==0)
        return;
    undo_counter++;
    int i,j;
    VER--;
    score -= points[VER];
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            game[i][j]=prev[VER][i][j];
        }
    }
}

void handle_redo()
{
    if(undo_counter == 0)
        return;
    undo_counter--;
    score += points[VER];
    VER++;
    int i,j;
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            game[i][j]=prev[VER][i][j];
        }
    }
}

void handle_right()
{
    if(can_handle() == false)
    {
        return;
    }
    push_right();
    combine_right();
    push_right();
    gen_new_block();
}

void handle_up()
{
    rotate_right();
    handle_right();
    rotate_right();
    rotate_right();
    rotate_right();
    update_version();
}

void handle_down()
{
    rotate_right();
    rotate_right();
    rotate_right();
    handle_right();
    rotate_right();
    update_version();
}

void handle_left()
{
    rotate_right();
    rotate_right();
    handle_right();
    rotate_right();
    rotate_right();
    update_version();
}

bool end_game()
{
    int i,j;
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            if(game[i][j]==0)
            {
                return false;
            }
            if(i<n)
            {
                if(game[i][j]==game[i+1][j])
                    return false;
            }
            if(j<n)
            {
                if(game[i][j]==game[i][j+1])
                    return false;
            }
        }
    }
    return true;
}

void new_game()
{
    VER = 0;
    undo_counter = 0;
    preparation();
}

std::string num_to_str(long long x)
{
    if(x==0)
    {
        return "0";
    }
    std::string str;
    while(x)
    {
        str+='0'+x%10;
        x/=10;
    }
    std::string ret;
    int i;
    for(i=str.size()-1;i>=0;i--)
    {
        ret += str[i];
    }
    return ret;
}

void print()
{
    int i,j;
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            std::cout<<game[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}


int main()
{
    srand(time(NULL));
    new_game();
    sf::RenderWindow window(sf::VideoMode(450, 500), "2048");
    window.setKeyRepeatEnabled(false);
    int ENDED = 0;
    int played = 0;
    sf::Font font;
    while(!font.loadFromFile("gobold.ttf"))
    {

    }
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        //played = 0;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed)
            {
                //played = 1;
                if(ENDED == 1 && event.key.code == sf::Keyboard::Q)
                {
                    window.close();
                }
                if(ENDED == 1 && event.key.code == sf::Keyboard::N)
                {
                    new_game();
                    ENDED = 0;
                }
                if(ENDED == 1 && event.key.code == sf::Keyboard::B)
                {
                    ENDED = 0;
                    handle_undo();
                }
                if(ENDED == 1)
                {
                    continue;
                }
                if(event.key.code == sf::Keyboard::Left)
                {
                    undo_counter = 0;
                    handle_left();
                }
                if(event.key.code == sf::Keyboard::Up)
                {
                    undo_counter = 0;
                    handle_up();
                }
                if(event.key.code == sf::Keyboard::Right)
                {
                    undo_counter = 0;
                    handle_right();
                    update_version();
                }
                if(event.key.code == sf::Keyboard::Down)
                {
                    undo_counter = 0;
                    handle_down();
                }
                if(event.key.code == sf::Keyboard::U)
                {
                    handle_undo();
                }
                if(event.key.code == sf::Keyboard::R)
                {
                    handle_redo();
                }
            }
        }
        if(!window.isOpen())
            break;
        window.clear(sf::Color::Black);

        if(end_game() == true)
        {
            ENDED = 1;
            sf::Text text;
            std::string to_print = "End of game";
            to_print += "\n";
            to_print += "Your score: ";
            to_print += num_to_str(score);
            to_print += "\n";
            to_print += "Press N for New game";
            to_print += "\n";
            to_print += "Press Q for Quit";
            to_print += "\n";
            to_print += "Press B for undo";
            text.setFont(font);
            text.setString(to_print);
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::Green);
            window.draw(text);

            window.display();
            continue;
        }
        //if(played)
        //    print();

        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=n;j++)
            {
                int c = game[i][j];
                sf::Sprite sprite;
                sf::Texture texture;
                std::string file_name = "images\\";
                file_name += num_to_str(c);
                file_name += ".png";
                while(!texture.loadFromFile(file_name))
                {

                }
                texture.setSmooth(true);
                sprite.setTexture(texture);
                sprite.setPosition((j-1)*100+j*10,(i-1)*100+i*10);

                ///display score
                sf::Text Score;
                Score.setFont(font);
                std::string dis_score;
                dis_score += "Score: ";
                dis_score += num_to_str(score);
                Score.setString(dis_score);
                Score.setCharacterSize(30);
                Score.setFillColor(sf::Color::Green);
                Score.setPosition(1,451);
                window.draw(Score);

                window.draw(sprite);
            }
        }

        window.display();
    }

    return 0;
}

