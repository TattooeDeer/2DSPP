#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <bits/stdc++.h> 


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
        int x_UL_coord;
        int y_UL_coord;
        int x_LR_coord;
        int y_LR_coord;

        // Indicador de si esta flipeado o no
        bool fliped = false;

        // Dimensiones del objeto
        int width;
        int height;

    public:
        // Constructor
        Item(int id_instance, int width_instance, int height_instance) {
            set_id(id_instance);
            set_width(width_instance);
            set_height(height_instance);
        }

        // Transpone el objeto, aplicar antes de el emplazamiento (asignar valores a (x,y) coords)
        void flip_item() {
            int tmp;
            tmp = width;
            width = height;
            height = tmp;
            fliped = true;
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
            x_UL_coord = x_LL_coord;
            x_UR_coord = new_coord + get_width();
            x_LR_coord = x_UR_coord;
        }
        void set_y_LL_coord(int new_coord) {
            y_LL_coord = new_coord;
            y_UR_coord = y_LL_coord + get_height();
            y_UL_coord = y_UR_coord;
            y_LR_coord = y_LL_coord;
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

        int get_x_UL_coord() const{
            return x_UL_coord;
        }

        int get_y_UL_coord() const{
            return y_UL_coord;
        }

        int get_y_LR_coord() const{
            return y_LR_coord;
        }

        int get_x_LR_coord() const{
            return x_LR_coord;
        }

        int get_id() const{
            return id;
        }

        bool get_fliped() const{
            return fliped;
        }
        void set_id(int new_id){
            id = new_id;
        } 
};

class Cinta {
    public:
        int total_height = 0;
        int total_width = 0;
        int used_area = 0;
        int max_width;
        int n_objetos;
        int start_placement_x; // Punto de origen desde el que comenzar a mover los articulos en la cinta, esq. sup. derecha
        int start_placement_y;
        int n_placed = 0;
        vector<int> final_placement;
        //list<Item> s_n; // Solución a evaluar --> creo que esto deberia estar en la funcion de HC
        list<Item> items_to_place;
        list<Item> placed_items;

        // Constructor
        Cinta(int n_obj, int ancho_cinta, list<Item> items){
            set_max_width(ancho_cinta);
            set_items_to_place(items);
            set_n_objetos(n_obj);
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

        // Nota: Al pasar solo las coordenadas ahorramos tiempo de procesamiento porque la cpu no tiene que copiar todo
        // El objeto Articulo
        bool try_down(Item item_to_place){
            bool overlap;
            if(item_to_place.get_y_LL_coord()-1 < 0){
                return false;
            }
            // Solo necesitamos chequear colisiones en las esquinas LL y LR cuando nos movemos hacia abajo
            for(auto const& placed_item : placed_items){
                bool overlap_LL_toPlace = item_to_place.get_x_LL_coord() < placed_item.get_x_UR_coord() && 
                                item_to_place.get_y_LL_coord()-1 < placed_item.get_y_UR_coord() &&
                                item_to_place.get_x_LL_coord() > placed_item.get_x_LL_coord() &&
                                item_to_place.get_y_LL_coord()-1 > placed_item.get_y_LL_coord();

                
                bool overlap_LR_toPlace = item_to_place.get_x_LR_coord() < placed_item.get_x_UR_coord() && 
                                item_to_place.get_y_LR_coord()-1 < placed_item.get_y_UR_coord() &&
                                item_to_place.get_x_LR_coord() > placed_item.get_x_LL_coord() &&
                                item_to_place.get_y_LR_coord()-1 > placed_item.get_y_LL_coord();

                // Ahora chequear si alguna esquina de un item ya emplazado entro en el item a emplazar
                bool overlap_UR_placed = (placed_item.get_x_UR_coord() < item_to_place.get_x_UR_coord()) && 
                                (placed_item.get_y_UR_coord() < item_to_place.get_y_UR_coord()-1) &&
                                (placed_item.get_x_UR_coord() > item_to_place.get_x_LL_coord()) &&
                                (placed_item.get_y_UR_coord() > item_to_place.get_y_LL_coord()-1);

                bool overlap_UL_placed = (placed_item.get_x_UL_coord() < item_to_place.get_x_UR_coord()) && 
                                (placed_item.get_y_UL_coord() < item_to_place.get_y_UR_coord()-1) &&
                                (placed_item.get_x_UL_coord() > item_to_place.get_x_LL_coord()) &&
                                (placed_item.get_y_UL_coord() > item_to_place.get_y_LL_coord()-1);

                if(overlap_LL_toPlace || overlap_LR_toPlace || overlap_UR_placed || overlap_UL_placed){
                    return false;
                }
            }
            return true;
        }

        bool try_left(auto item_to_place){
            bool overlap;
            if(item_to_place.get_x_LL_coord()-1 < 0){
                return false;
            }
            // Solo necesitamos chequear colisiones en las esquinas LL y UL cuando nos movemos hacia el lado
            for(auto const& placed_item : placed_items){
                bool overlap_LL_toPlace = (item_to_place.get_x_LL_coord()-1 < placed_item.get_x_UR_coord()) && 
                                (item_to_place.get_y_LL_coord() < placed_item.get_y_UR_coord()) &&
                                (item_to_place.get_x_LL_coord()-1 > placed_item.get_x_LL_coord()) &&
                                (item_to_place.get_y_LL_coord() > placed_item.get_y_LL_coord());

                bool overlap_UL_toPlace = (item_to_place.get_x_UL_coord()-1 < placed_item.get_x_UR_coord()) && 
                                (item_to_place.get_y_UL_coord() < placed_item.get_y_UR_coord()) &&
                                (item_to_place.get_x_UL_coord()-1 > placed_item.get_x_LL_coord()) &&
                                (item_to_place.get_y_UL_coord() > placed_item.get_y_LL_coord());

                // Ahora chequear si alguna esquina de un item ya emplazado entro en el item a emplazar
                bool overlap_LR_placed = (placed_item.get_x_LR_coord() < item_to_place.get_x_UR_coord()-1) && 
                                (placed_item.get_y_LR_coord() < item_to_place.get_y_UR_coord()) &&
                                (placed_item.get_x_LR_coord() > item_to_place.get_x_LL_coord()-1) &&
                                (placed_item.get_y_LR_coord() > item_to_place.get_y_LL_coord());

                bool overlap_UR_placed = (placed_item.get_x_UR_coord() < item_to_place.get_x_UR_coord()-1) && 
                                (placed_item.get_y_UR_coord() < item_to_place.get_y_UR_coord()) &&
                                (placed_item.get_x_UR_coord() > item_to_place.get_x_LL_coord()-1) &&
                                (placed_item.get_y_UR_coord() > item_to_place.get_y_LL_coord());

                if(overlap_LL_toPlace || overlap_UL_toPlace || overlap_LR_placed || overlap_UR_placed){
                    return false;
                }
            }
            return true;

        }

        Item get_item_by_id(int id){
            for(auto const& item : items_to_place){
                if(item.get_id() == id){
                    return item;
                }
            }

        }
        
        void apply_BL(Item articulo){
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
            int pre_insert_height = get_total_height();
            bool fit_with_flip = true;
            bool fit_without_flip = true;
            int altura_with_flip, altura_without_flip;

            Item without_flip = articulo;
            Item with_flip = articulo;
            with_flip.flip_item();

            // Posicion inicial de los objetos
            without_flip.place_item(get_max_width()-without_flip.get_width(), get_total_height()+1);
            with_flip.place_item(get_max_width()-with_flip.get_width(), get_total_height()+1);
            //cout << "----------------------------------------"<< endl;
            //cout << "Max width: " << get_max_width() << endl;
            //cout << "Total Height: " << get_total_height() << endl;
            //cout << "Id articulo: " << articulo.get_id() << endl;
            if(without_flip.get_x_LL_coord() < 0){
                fit_without_flip = false;
            }
            if(with_flip.get_x_LL_coord() < 0){
                fit_with_flip = false;
            }
            //if(fit_with_flip){
            //    cout << "Posicion inicial con flip: "<< endl;
            //    cout << "x_LL: " << with_flip.get_x_LL_coord() << endl;
            //    cout << "y_LL: " << with_flip.get_y_LL_coord() << endl;
            //}
            //if(fit_without_flip){
            //    cout << "Posicion inicial sin flip: " << endl;
            //    cout << "x_LL: " << without_flip.get_x_LL_coord() << endl;
            //    cout << "y_LL: " << without_flip.get_y_LL_coord() << endl;
            //}
            //cout << "...." << endl;


            // articulo sin flip
            //cout << "Item ID: " << articulo.get_id() << endl;
            if(fit_without_flip){
                while(!final_flag){ // O(ancho_cinta*altura_emplazado)
                    while(!tope_horizontal){ // O(ancho_cinta-ancho_item)
                        while(!tope_vertical){ // O(altura_emplazado)
                            if(try_down(without_flip)){
                                //cout << "movimiento hacia abajo"<< endl;
                                without_flip.move_down();
                                //cout << "y_LL: " << without_flip.get_y_LL_coord();
                            }
                            else{
                                tope_vertical = true;
                            }
                        }
                        if(try_left(without_flip)){
                            //cout << "Movimiento hacia la izquierda" << endl;
                            without_flip.move_left();
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
                altura_without_flip =  max(pre_insert_height, without_flip.get_y_UR_coord());
                //cout << "altura without flip: " << altura_without_flip << endl;

            }
            final_flag = false;
            tope_vertical = false;
            tope_horizontal = false;

            // articulo con flip
            if(fit_with_flip){
                while(!final_flag){ // O(ancho_cinta*altura_emplazado)
                    while(!tope_horizontal){ // O(ancho_cinta-ancho_item)
                        while(!tope_vertical){ // O(altura_emplazado)
                            if(try_down(with_flip)){
                                with_flip.move_down();
                            }
                            else{
                                tope_vertical = true;
                            }
                        }
                        if(try_left(with_flip)){
                            with_flip.move_left();
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

                altura_with_flip =  max(pre_insert_height, with_flip.get_y_UR_coord());
                //cout << "altura with flip: " << altura_with_flip << endl;
            }
            // actualizacion del articulo e insercion a la lista de emplazados
            if(fit_with_flip && fit_without_flip){
                if(altura_without_flip <= altura_with_flip){
                    //cout << "Mejor sin flip"<< endl;
                    articulo.set_x_LL_coord(without_flip.get_x_LL_coord());
                    articulo.set_y_LL_coord(without_flip.get_y_LL_coord());
                    articulo.set_width(without_flip.get_width());
                    articulo.set_height(without_flip.get_height());
                }
                else{
                    //cout << "Mejor con flip"<< endl;
                    articulo.set_x_LL_coord(with_flip.get_x_LL_coord());
                    articulo.set_y_LL_coord(with_flip.get_y_LL_coord());
                    articulo.set_width(with_flip.get_width());
                    articulo.set_height(with_flip.get_height());
                }
            }
            else if(fit_with_flip && !fit_without_flip){
                //cout << "Mejor con flip"<< endl;
                articulo.set_x_LL_coord(with_flip.get_x_LL_coord());
                articulo.set_y_LL_coord(with_flip.get_y_LL_coord());
                articulo.set_width(with_flip.get_width());
                articulo.set_height(with_flip.get_height());
            }
            else if(fit_without_flip && !fit_with_flip){
                //cout << "Mejor sin flip"<< endl;
                articulo.set_x_LL_coord(without_flip.get_x_LL_coord());
                articulo.set_y_LL_coord(without_flip.get_y_LL_coord());
                articulo.set_width(without_flip.get_width());
                articulo.set_height(without_flip.get_height());
            }
            else{
                cout << "El objeto de ID " << articulo.get_id() << "Es demasiado grande para la cinta";
                return;
            }
            append_placed_item(articulo);
            //cout << "Posicion final: "<< endl;
            //cout << "x_LL: " << articulo.get_x_LL_coord() << endl;
            //cout << "y_LL: " << articulo.get_y_LL_coord() << endl;
            //cout << "x_UR: " << articulo.get_x_UR_coord() << endl;
            //cout << "y_UR: " << articulo.get_y_UR_coord() << endl;
        }

        void append_placed_item(Item item){
            placed_items.push_back(item);
            set_total_height(max(item.get_y_UR_coord(), get_total_height()));
            set_used_area(get_used_area() + item.get_height()*item.get_width());
            n_placed++;
        }
        
        // Setters & Getters
        void set_items_to_place(list<Item> lista_items){
            items_to_place = lista_items;
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

        int get_n_objetos(){
            return n_objetos;
        }
        
        void set_max_width(int width){
            max_width = width;
        }

        int get_max_width(){
            return max_width;
        }

        void set_used_area(int new_area){
            used_area = new_area;
        }

        int get_used_area(){
            return used_area;
        }

        void set_total_height(int new_height){
            total_height = new_height;
        }

        int get_total_height(bool set_total_height = false){ // Esta es la funcion de evaluacion vainilla
            int top_y_UR_corner = 0;
            for(auto const& item : placed_items){
                //cout << "placed_item height: " << item.get_y_UR_coord() << endl;
               if(item.get_y_UR_coord() > top_y_UR_corner){
                   top_y_UR_corner = item.get_y_UR_coord();
               }
           }
           if(set_total_height){
               total_height = top_y_UR_corner; 
           }

           return top_y_UR_corner;
        }

        vector<int> get_final_placement(bool print_secuence = true){
            //return final_placement;
            if(print_secuence){
                for(auto const& item: placed_items){
                    cout << item.get_id() << "  ";
                }
                cout << endl;    
            }
            return final_placement;
        }

        void set_final_placement(vector<int> sol){
            final_placement = sol;
            //for(auto const& item: placed_items){
            //    //cout << item.get_id() << "  ";
//
            //}
            //cout << endl;
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

/*
void display(int a[], int n) 
{ 
    for (int i = 0; i < n; i++) { 
        cout << a[i] << "  "; 
    } 
    cout << endl; 
} */

void display(vector<int> ids_vector) 
{ 
    for (auto const& id: ids_vector) { 
        cout << id << "  "; 
    } 
    cout << endl; 
} 
//Hill Climbing + MM

void hillClimbing_MM(Cinta& cinta_inst){ // Por ahora lo implementare sin Restart --> TODO
    std::srand(unsigned(std::time(0)));
    // Representacion: Lista de id's de los items en el orden de insercion
    int items_id[cinta_inst.get_n_objetos()];
    int best_order[cinta_inst.get_n_objetos()];
    int best_height = 9999; // por ahora fijare esto asi, lo ideal sería fijarlo de acuerdo a alguna cota superior
    bool local = false; // minimo local
    vector<int> initial_ids; 
    vector<int> s_c; // Solucion Actual, se inicializa con un orden aleatorio de items

    for(auto const& item : cinta_inst.items_to_place){
        s_c.push_back(item.get_id());
    }
    random_shuffle(s_c.begin(), s_c.end()); // inicializacion aleatoria de s_c
    cout << "Ids iniciales: ";
    display(s_c);
    Cinta tmp_cinta = cinta_inst;
    for(int const& id: s_c){
        tmp_cinta.apply_BL(cinta_inst.get_item_by_id(id));
    }

    int f_s_c = tmp_cinta.get_total_height();

    int n = cinta_inst.get_n_objetos();
    //sort(s_c, s_c + n);
    int best_f_s_n = f_s_c;
    vector<int> best_s_n = s_c;
    int max_iter = 10;
    int n_iter = 0;
    do{
        for(int i_left = 0; i_left < n-1; i_left = i_left+1){
            for(int i_right = 0; i_right < n; i_right = i_right+1){ // El vecindario esta definido por un solo swap entre dos elementos del arreglo de ids
                vector<int> tmp_ids = s_c; // 1. Hacer una copia del arreglo original
                Cinta tmp_cinta = cinta_inst; // Hacer una copia de la cinta
                swap(tmp_ids[i_left], tmp_ids[i_right]); // 2. Hacer swap de las posiciones i_left con i_right, este es el movimiento
                //cout << "Probando: ";
                //display(tmp_ids);
                for(int const& item_id: tmp_ids){
                    tmp_cinta.apply_BL(tmp_cinta.get_item_by_id(item_id)); // 4. Hacer BL sobre la copia con swap y la cinta
                }
                int f_s_n_tmp = tmp_cinta.get_total_height(); // valor de la funcion de evaluacion para cada movimiento
                //cout << "Altura alcanzada: " <<  f_s_n_tmp;
                //cout << endl;
                if(f_s_n_tmp < best_f_s_n){
                    best_f_s_n = f_s_n_tmp;
                    best_s_n = tmp_ids;
                }
            }
        }
        if(best_f_s_n <= f_s_c){ // Actualizar s_c
            //cout << "Best f_s_n: " << best_f_s_n << endl;
            //cout << "Best f_s_c: " << f_s_c << endl;
            f_s_c = best_f_s_n;
            s_c = best_s_n;
            n_iter++;
        }
        else{
            local = true;
        }
        
    }while(!local && n_iter <= max_iter);

    // Actualizar la instancia de la cinta original con el orden encontrado de articulos y aplicar BL para dejarlos emplazados
    cinta_inst.set_final_placement(s_c);
    
    vector<int> final_placement = cinta_inst.get_final_placement();
    cout << "---------- RESULTADO FINAL ----------" << endl;
    cout << "Emplazamiento final: ";
    display(cinta_inst.get_final_placement());
    cout << endl;
    for(int const& id: final_placement){
        cout << "Ingresando: " << id << endl;
        cinta_inst.apply_BL(cinta_inst.get_item_by_id(id));
    }
    //for(int const& item_id: s_c){
    //    cinta_inst.append_placed_item(cinta_inst.get_item_by_id(item_id));
    //    cout << "Id: " << item_id << endl;
    //    cout << "x_LL: " << cinta_inst.get_item_by_id(item_id).get_x_LL_coord() << endl;
    //    cout << "y_LL: " << cinta_inst.get_item_by_id(item_id).get_y_LL_coord() << endl;
    //    cout << "Fliped: " << cinta_inst.get_item_by_id(item_id).get_fliped() << endl;
    //    cout << "-------" << endl;
    //}
    cout << "Cantidad de items en la lista de emplazados: " << cinta_inst.n_placed << endl;
    cout << "Altura Final Alcanzada: " << cinta_inst.get_total_height() << endl;
}

// Parsea la linea y retorna un struct con la info para inicializar los objetos a emplazar -- DEPRECATED
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
}


// Lee el archivo de la instancia y genera una lista de objetos a emplazar
data_instancia read_instance(string file_path_buff = "test.txt"){
    data_instancia instancia;
    ifstream infile(file_path_buff.c_str());
    int line_number = 0;
    string line;
    list<Item> objetos;

    while(!infile.eof()) {
        getline(infile, line);
        std::stringstream  linestream(line);

        if (line_number == 0){
            instancia.n_objetos = atoi(line.c_str());
        }
        else if (line_number == 1){
            instancia.width = atoi(line.c_str());
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
    //cout << data.n_objetos;
    Cinta cinta_instancia(data.n_objetos, data.width, data.instance_obj);
    hillClimbing_MM(cinta_instancia);
    
    return 0;
}

