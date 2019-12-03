#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


class Item {
    private:
        // id objeto
        int id;

        // Coordenadas esquinas inferior izquierda y superior derecha del objeto
        int x_LL_coord;
        int y_LL_coord;
        int x_UR_coord;
        int y_UR_coord;

        // Dimensiones del objeto
        int width;
        int height;

    public:
        // Constructor
        Item(int id_instance, int width_instance, int height_instance) {
            id = id_instance;
            width = width_instance;
            height = height_instance;
        }

        // Transpone el objeto, aplicar antes de el emplazamiento (asignar valores a (x,y) coords)
        void flip_item() {
            int tmp;
            tmp = width;
            width = height;
            height = tmp;
        }

        void move_down(){
            place_item(x_LL_coord, y_LL_coord-1);
        }

        void move_left(){
            place_item(x_LL_coord-1, y_LL_coord);
        }

        // Asigna coordenadas a las cuatro esquinas de un objeto
        void place_item(int x_coord_new, int y_coord_new){
            set_x_LL_coord(x_coord_new);
            set_y_LL_coord(y_coord_new);
            calculate_x_UR_coord();
            calculate_y_UR_coord();
        }

        void calculate_x_UR_coord(){
            x_UR_coord = x_LL_coord + get_width();
        }

        void calculate_y_UR_coord(){
            y_UR_coord = y_LL_coord + get_height();
        }

        // Setters & Getters
        void set_width(int new_width) {
            width = new_width;
        }
        void set_height(int new_height) {
            height = new_height;
        }
        void set_x_LL_coord(int new_coord) {
            x_LL_coord = new_coord;

        }
        void set_y_LL_coord(int new_coord) {
            y_LL_coord = new_coord;
        }

        int get_width() const{
            return width;
        }
        int get_height() const{
            return height;
        }
        int get_x_LL_coord() const{
            return x_LL_coord;
        }
        int get_y_LL_coord() const{
            return y_LL_coord;
        }
        int get_x_UR_coord() const{
            return x_UR_coord;
        }
        int get_y_UR_coord() const{
            return y_UR_coord;
        }
};

class Cinta {
    public:
        int total_height;
        int total_width;
        int used_area;
        int max_width;
        int n_objetos;
        int start_placement_x; // Punto de origen desde el que comenzar a mover los articulos en la cinta, esq. sup. derecha
        int start_placement_y;
        //list<Item> s_n; // Solución a evaluar --> creo que esto deberia estar en la funcion de HC
        list<Item> items_to_place;
        list<Item> placed_items;

        // Constructor
        Cinta(int n_obj, int ancho_cinta, list<Item> items){
            set_max_width(ancho_cinta);
            set_items_to_place(items);
            set_n_objetos(n_obj);
        }

        int calculate_total_height(bool set_total_height = true){
            int top_y_UR_corner = 0;
            for(auto const& item : placed_items){
               if(item.get_y_UR_coord() > top_y_UR_corner){
                   top_y_UR_corner = item.get_y_UR_coord();
               }
           }
           if(set_total_height){
               total_height = top_y_UR_corner; 
           }

           return top_y_UR_corner;
        }

        int calculate_total_width(bool set_total_width = true){
            int top_x_UR_corner = 0;
            for(auto const& item : placed_items){
               if(item.get_x_UR_coord() > top_x_UR_corner){
                   top_x_UR_corner = item.get_x_UR_coord();
               }
           }
           if(set_total_width){
               total_width = top_x_UR_corner; 
           }

           return top_x_UR_corner;

        }
        bool try_down(Item articulo); // TODO
        bool try_left(Item articulo); // TODO
        
        int apply_BL(Item &articulo){
            /*
            1. calcular la posicion de inicio del artículo, setear flag tope_horizontal = false tope_vertical = false
            2. bajar en 1
            3. Chequear si esta dentro de un objeto o fuera de la cinta
            4. Si está dentro: abortar y pasar a 6, tope_vertical = true
            5. Si no está adentro repetir 2
            6. mover 1 hacia la izquiera
            7. Chequear si está dentro o fuera de la cinta
            8. Si está adentro abortar y tope_horizontal = true, check tope_horizontal and tope_vertical = final, saltar a 10
            9. Si no está adentro, tope_horizontal = false y tope_vertical = false, saltar a 2
            10. Calcular Altura total del empacado y guardarla en una variable junto con la instancia del objeto en la posicion final
            11. Hacer flip al objeto y empezar desde 1
            12. Comparar las alturas máximas de los empaques, elegir la que tenga menor altura
            13. Actualizar instancia del objeto y appendearlo a lista de placed_items en la cinta, quitarlo de l
            
            */
            bool final_flag = false;
            bool tope_vertical = false;
            bool tope_horizontal = false;

            while(!final_flag){
                while(!tope_horizontal){
                    while(!tope_vertical){
                        if(try_down(articulo)){
                            articulo.move_down();
                        }
                        else{
                            tope_vertical = true;
                        }
                    }
                    if(try_left(articulo)){
                        articulo.move_left();
                        tope_vertical = false;
                    }
                    else{
                        tope_horizontal = true;
                    }
                }
                if(tope_vertical && tope_horizontal){
                    final_flag = true;
                }
            }
        }

        void append_placed_item(Item item){
            placed_items.push_back(item);
        }
        
        // Setters & Getters
        void set_items_to_place(list<Item> lista_items){
            items_to_place = lista_items;
        }
        
        void set_max_width(int width){
            max_width = width;
        }

        int calculate_used_area(int area, bool set_used_area = true){
            int total_area = 0;
           for(auto const& item : placed_items){
               total_area += (item.get_width()*item.get_height());
           }

           if(set_used_area){
               used_area = total_area;
           }

           return total_area; 
        }

        void set_n_objetos(int n_obj){
            n_objetos = n_obj;
        }
        void set_start_placement_x(int start_x); // TODO

        void set_start_placement_y(int start_y); // TODO

        
};


struct data_instancia {
        int n_objetos;
        int width;
        list<Item> instance_obj;
};
// Funciones utiles

//Hill Climbing + MM

auto hillClimbing_MM(Cinta cinta, list<Item> items); // TODO

// Parsea la linea y retorna un struct con la info para inicializar los objetos a emplazar
auto item_specs(string line){
    size_t pos = 0;
    string token;
    string delimiter = " ";
    int data_array[3];
    int i = 0;

    struct obj_data {
        int id;
        int width;
        int height;
    };

    // Leer por linea y extraer info de los objetos
    

    //while ((pos = line.find(delimiter)) != std::string::npos) {
    //    token = line.substr(0, pos);
    //    cout << "\nLinea:"+ line;
    //    cout << "\nToken:" + token;
    //    data_array[i] = atoi(token.c_str());
    //    //std::cout << token << std::endl;
    //    line.erase(0, pos + delimiter.length());
    //    cout << "\nPor leer: "+ line; 
    //    i++;
    //}
    ////cout << data_array[0];
    ////cout << data_array[1];
    ////cout << data_array[2];
    //return obj_data {data_array[0], data_array[1], data_array[2]}; // data de inicializacion del objeto
}


// Lee el archivo de la instancia y genera una lista de objetos a emplazar
auto read_instance(string file_path_buff = "test.txt"){
    data_instancia instancia;
    ifstream infile(file_path_buff.c_str());
    int line_number = 0;
    string line;
    //int n_obj;
    //int ancho_cinta;
    list<Item> objetos;

    while(!infile.eof()) {
        getline(infile, line);
        std::stringstream  linestream(line);

        if (line_number == 0){
            instancia.n_objetos << atoi(line.c_str());
        }
        else if (line_number == 1){
            instancia.width << atoi(line.c_str());
        }
        else{
            int item_id, item_width, item_height;

            linestream >> item_id >> item_width >> item_height;
            Item dummy_obj(item_id, item_width, item_height);
            objetos.push_back(dummy_obj);
        }
        line_number++;
    }
    infile.close();
    instancia.instance_obj = objetos;

    return instancia;
}




// Main
int main() {
    data_instancia data;
    data = read_instance();
    Cinta this_cinta(data.n_objetos, data.width, data.instance_obj);
    //cout << items_instancia.size();
    //list<Item>::iterator it;
    //instancia = read_instance();
//
    //return 0;
//
//
    //for (it = items_instancia.begin(); it != items_instancia.end(), it++){
    //    //Cosas de iteraciones
    //}

    return 0;
}

