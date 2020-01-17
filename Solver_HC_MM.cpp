#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <bits/stdc++.h>
#include <chrono>



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
        bool flipped = false;

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
            flipped = true;
        }

        void move_down(){
            int new_coord = get_y_LL_coord() -1;
            place_item(x_LL_coord, new_coord);
        }

        void move_left(){
            int new_coord = get_x_LL_coord() -1;
            place_item(new_coord, y_LL_coord);
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
            x_UL_coord = new_coord;
            x_UR_coord = new_coord + get_width();
            x_LR_coord = new_coord + get_width();
        }
        void set_y_LL_coord(int new_coord) {
            y_LL_coord = new_coord;
            y_UR_coord = new_coord + get_height();
            y_UL_coord =new_coord + get_height();
            y_LR_coord = new_coord;
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

        void set_id(int new_id){
            id = new_id;
        }
        
        bool get_flipped() const{
            return flipped;
        }

        void set_flipped(bool new_flip){
            flipped = new_flip;
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
        bool try_down(Item item_to_place, bool verbose = false){
            bool overlap;
            if(item_to_place.get_y_LL_coord()-1 < 0){
                return false;
            }
            // Solo necesitamos chequear colisiones en las esquinas LL y LR cuando nos movemos hacia abajo
            if(verbose){
                cout << "Items placed till now: " << endl;
                for(auto const& placed_item : placed_items){
                    cout << "id: " << placed_item.get_id() << endl;
                    cout << "\t LL: (" << placed_item.get_x_LL_coord() << ", " << placed_item.get_y_LL_coord() << ")" << endl;
                    cout << "\t UR: (" << placed_item.get_x_UR_coord() << ", " << placed_item.get_y_UR_coord() << ")" << endl;
                }
            }


            for(auto const& placed_item : placed_items){
                // Estas condiciones chequean si se llegan a sobreponer el articulo a emplazar
                // con alguno de los emplazados ya en la cinta
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

                float mid_point_x = (item_to_place.get_x_LL_coord() + item_to_place.get_x_LR_coord())/2;
                float mid_point_y = item_to_place.get_y_LL_coord()-1;
                bool overlap_perfect_fit = (mid_point_x > placed_item.get_x_LL_coord() &&
                                mid_point_x < placed_item.get_x_LR_coord() &&
                                mid_point_y < placed_item.get_y_UR_coord() &&
                                mid_point_y > placed_item.get_y_LR_coord()); 

                if(overlap_LL_toPlace || overlap_LR_toPlace || overlap_UR_placed || overlap_UL_placed || overlap_perfect_fit){
                    return false;
                }
            }
            return true;
        }

        bool try_left(auto item_to_place, bool verbose = false){
            bool overlap;
            if(item_to_place.get_x_LL_coord()-1 < 0){
                return false;
            }
            if(verbose){
                cout << "Items placed till now: " << endl;
                for(auto const& placed_item : placed_items){
                    cout << "id: " << placed_item.get_id() <<endl;
                    cout << "\t LL: (" << placed_item.get_x_LL_coord() << ", " << placed_item.get_y_LL_coord() << ")" << endl;
                    cout << "\t UR: (" << placed_item.get_x_UR_coord() << ", " << placed_item.get_y_UR_coord() << ")" << endl;
                }
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

                
                float mid_point_y = (item_to_place.get_y_LL_coord() + item_to_place.get_y_UL_coord())/2;
                float mid_point_x = item_to_place.get_x_LL_coord()-1;
                bool overlap_perfect_fit = (mid_point_x < placed_item.get_x_LR_coord() &&
                                mid_point_x > placed_item.get_x_LL_coord() &&
                                mid_point_y < placed_item.get_y_UR_coord() &&
                                mid_point_y > placed_item.get_y_LL_coord()); 

                if(overlap_LL_toPlace || overlap_UL_toPlace || overlap_LR_placed || overlap_UR_placed || overlap_perfect_fit){
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
        Item get_placed_item_by_id(int id){
            for(auto const& item : placed_items){
                if(item.get_id() == id){
                    return item;
                }
            }

        }
        
        void apply_BL(Item articulo, bool verbose = false){
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
            if(verbose){
                cout << "Altura máxima hasta ahora: "<< get_total_height() << endl;
                cout << "Items placed till now: " << endl;
                for(auto const& placed_item : placed_items){
                    cout << "id: " << placed_item.get_id() <<endl;
                    cout << "\t LL: (" << placed_item.get_x_LL_coord() << ", " << placed_item.get_y_LL_coord() << ")" << endl;
                    cout << "\t UR: (" << placed_item.get_x_UR_coord() << ", " << placed_item.get_y_UR_coord() << ")" << endl;
                }
                }
            without_flip.place_item(get_max_width()-without_flip.get_width(), get_total_height()+1);
            with_flip.place_item(get_max_width()-without_flip.get_height(), get_total_height()+1);
            if(without_flip.get_x_LL_coord() < 0){
                fit_without_flip = false;
            }
            if(with_flip.get_x_LL_coord() < 0){
                fit_with_flip = false;
            }

            if(fit_with_flip && verbose){
                cout << "Posicion inicial con flip: "<< endl;
                cout << "LL: " << with_flip.get_x_LL_coord() << ", "  << with_flip.get_y_LL_coord() << endl;
                cout << "UR: " << with_flip.get_x_UR_coord() << ", "  << with_flip.get_y_UR_coord()<< endl;
            }
            if(fit_without_flip && verbose){
                cout << "Posicion inicial sin flip: " << endl;
                 cout << "LL: " << without_flip.get_x_LL_coord() << ", "  << without_flip.get_y_LL_coord() << endl;
                cout << "UR: " << without_flip.get_x_UR_coord() << ", "  << without_flip.get_y_UR_coord()<< endl;
            }


            // articulo sin flip
            if(fit_without_flip){
                if(verbose){cout << "#### Without Flip" << endl;}
                while(!final_flag){ // O(ancho_cinta*altura_emplazado)
                    while(!tope_horizontal){ // O(ancho_cinta-ancho_item)
                        while(!tope_vertical){ // O(altura_emplazado)
                            if(try_down(without_flip, verbose = verbose)){
                                if(verbose){
                                    cout << "movimiento hacia abajo: "<< endl;
                                    cout << "Posicion actual: " << without_flip.get_x_LL_coord() << ", "  << without_flip.get_y_LL_coord() << " // "<< without_flip.get_x_UR_coord() << ", "  << without_flip.get_y_UR_coord() << endl;
                                    without_flip.move_down();
                                    cout << "Posicion nueva: " << without_flip.get_x_LL_coord() << ", "  << without_flip.get_y_LL_coord() << " // "<< without_flip.get_x_UR_coord() << ", "  << without_flip.get_y_UR_coord() << endl;
                                    }
                                else{without_flip.move_down();}
                            }
                            else{
                                tope_vertical = true;
                            }
                        }
                        if(try_left(without_flip, verbose = verbose)){
                            if(verbose){
                                cout << "movimiento hacia izquierda: "<< endl;
                                cout << "Posicion actual: " << without_flip.get_x_LL_coord() << ", "  << without_flip.get_y_LL_coord() << " // "<< without_flip.get_x_UR_coord() << ", "  << without_flip.get_y_UR_coord() << endl;
                                without_flip.move_left();
                                cout << "Posicion nueva: " << without_flip.get_x_LL_coord() << ", "  << without_flip.get_y_LL_coord() << " // "<< without_flip.get_x_UR_coord() << ", "  << without_flip.get_y_UR_coord() << endl;
                                }
                            else{without_flip.move_left();}
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
                if(verbose){cout << "altura without flip: " << altura_without_flip << endl;}

            }
            final_flag = false;
            tope_vertical = false;
            tope_horizontal = false;

            // articulo con flip
            if(fit_with_flip){
                if(verbose){cout << "#### With Flip" << endl;}
                while(!final_flag){ // O(ancho_cinta*altura_emplazado)
                    while(!tope_horizontal){ // O(ancho_cinta-ancho_item)
                        while(!tope_vertical){ // O(altura_emplazado)
                            if(try_down(with_flip, verbose = verbose)){
                                if(verbose){
                                    cout << "movimiento hacia abajo: "<< endl;
                                    cout << "Posicion actual: " << with_flip.get_x_LL_coord() << ", "  << with_flip.get_y_LL_coord()<< " // "<<with_flip.get_x_UR_coord() << ", "  << with_flip.get_y_UR_coord() << endl;
                                    with_flip.move_down();
                                    cout << "Posicion nueva: " << with_flip.get_x_LL_coord() << ", "  << with_flip.get_y_LL_coord()<< " // "<<with_flip.get_x_UR_coord() << ", "  << with_flip.get_y_UR_coord() << endl;
                                    }
                                else{
                                    with_flip.move_down();

                                }
                            }
                            else{
                                tope_vertical = true;
                            }
                        }
                        if(try_left(with_flip, verbose = verbose)){
                            
                            if(verbose){
                                cout << "movimiento hacia izquierda: "<< endl;
                                cout << "Posicion actual: " << with_flip.get_x_LL_coord() << ", "  << with_flip.get_y_LL_coord()<< " // "<< with_flip.get_x_UR_coord() << ", "  << with_flip.get_y_UR_coord() << endl;
                                with_flip.move_left();
                                cout << "Posicion nueva: " << with_flip.get_x_LL_coord() << ", "  << with_flip.get_y_LL_coord()<< " // "<< with_flip.get_x_UR_coord() << ", "  << with_flip.get_y_UR_coord() << endl;

                                }
                            else{with_flip.move_left();}
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
                if(verbose){cout << "altura with flip: " << altura_with_flip << endl;}
            }
            // actualizacion del articulo e insercion a la lista de emplazados
            // Si cabe de ambas formas comparamos alturas
            if(fit_with_flip && fit_without_flip){
                if(altura_without_flip <= altura_with_flip){
                    if(verbose){
                        cout << "Mejor sin flip"<< endl;
                        cout << "Se va a ingresar en LL: " <<without_flip.get_x_LL_coord() << ", " << without_flip.get_y_LL_coord()<<endl;
                        cout << "Se va a ingresar en UR: " <<without_flip.get_x_UR_coord() << ", " << without_flip.get_y_UR_coord()<<endl;
                        }
                    articulo.set_width(without_flip.get_width());
                    articulo.set_height(without_flip.get_height());
                    articulo.set_flipped(without_flip.get_flipped());
                    articulo.set_x_LL_coord(without_flip.get_x_LL_coord());
                    articulo.set_y_LL_coord(without_flip.get_y_LL_coord());
                    if(verbose){cout << "Flipped: " << articulo.get_flipped() << endl;}
                }
                else{
                    if(verbose){
                        cout << "Mejor con flip"<< endl;
                        cout << "Se va a ingresar en LL: " <<with_flip.get_x_LL_coord() << ", " << with_flip.get_y_LL_coord()<<endl;
                        cout << "Se va a ingresar en UR: " <<with_flip.get_x_UR_coord() << ", " << with_flip.get_y_UR_coord()<<endl;
                        }
                    articulo.set_width(with_flip.get_width());
                    articulo.set_height(with_flip.get_height());
                    articulo.set_flipped(with_flip.get_flipped());
                    articulo.set_x_LL_coord(with_flip.get_x_LL_coord());
                    articulo.set_y_LL_coord(with_flip.get_y_LL_coord());
                    if(verbose){cout << "Flipped: " << articulo.get_flipped() << endl;}
                }
            }
            // Si no cabe sin flip solo consideramos el con flip
            else if(fit_with_flip && !fit_without_flip){
                if(verbose){
                    cout << "Cabe solo con flip"<< endl;
                    cout << "Se va a ingresar en LL: " <<with_flip.get_x_LL_coord() << ", " << with_flip.get_y_LL_coord()<<endl;
                    cout << "Se va a ingresar en UR: " <<with_flip.get_x_UR_coord() << ", " << with_flip.get_y_UR_coord()<<endl;
                    }
                articulo.set_width(with_flip.get_width());
                articulo.set_height(with_flip.get_height());
                articulo.set_flipped(with_flip.get_flipped());
                articulo.set_x_LL_coord(with_flip.get_x_LL_coord());
                articulo.set_y_LL_coord(with_flip.get_y_LL_coord());
                if(verbose){cout << "Flipped: " << articulo.get_flipped() << endl;}
            }
            else if(fit_without_flip && !fit_with_flip){
                if(verbose){
                    cout << "Cabe solo sin flip"<< endl;
                    cout << "Se va a ingresar en LL: " <<without_flip.get_x_LL_coord() << ", " << without_flip.get_y_LL_coord()<<endl;
                    cout << "Se va a ingresar en UR: " <<without_flip.get_x_UR_coord() << ", " << without_flip.get_y_UR_coord()<<endl;
                    }
                articulo.set_width(without_flip.get_width());
                articulo.set_height(without_flip.get_height());
                articulo.set_flipped(without_flip.get_flipped());
                articulo.set_x_LL_coord(without_flip.get_x_LL_coord());
                articulo.set_y_LL_coord(without_flip.get_y_LL_coord());
                if(verbose){cout << "Flipped: " << articulo.get_flipped() << endl;}
            }
            else{
                cout << "El objeto de ID " << articulo.get_id() << " es demasiado grande para la cinta" << endl;
                return;
            }
            append_placed_item(articulo);
            if(verbose){
                cout << "Posicion final: "<< endl;
                cout << "LL: " << articulo.get_x_LL_coord() << ", " <<articulo.get_y_LL_coord() << endl;
                cout << "UR: " << articulo.get_x_UR_coord() << ", " <<articulo.get_y_UR_coord() << endl;

            } 
        }

        void append_placed_item(Item item){ // inserta un articulo en la lista de emplazados hasta el momento
            placed_items.push_back(item);
            set_total_height(max(item.get_y_UR_coord(), get_total_height()));
            set_used_area(get_used_area() + item.get_height()*item.get_width());
            n_placed++;
        }

        
        // Setters & Getters
        void set_items_to_place(list<Item> lista_items){
            items_to_place = lista_items;
        }

        int calculate_used_area(bool set_used_area = true){
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

        int get_total_height(bool set_total_height = false){ // Esta es la funcion de evaluacion de la solucion
            if(total_height != 0){
                return total_height;
            }
            else{
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
        }

        vector<int> get_final_placement(bool print_secuence = false){
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
        }

        list<Item> get_placed_items(){
            return placed_items;
        }
        
};


// Funciones y estructuras utiles
struct data_instancia {
        int n_objetos;
        int width;
        list<Item> instance_obj;
};

void display(vector<int> ids_vector) 
{ 
    for (auto const& id: ids_vector) { 
        cout << id << "  "; 
    } 
    cout << endl; 
}


//Hill Climbing + MM

void hillClimbing_MM(Cinta& cinta_inst, int n_restarts = 5){
    // Representacion: Lista de id's de los items en el orden de insercion
    int items_id[cinta_inst.get_n_objetos()];
    int best_order[cinta_inst.get_n_objetos()];
    vector<int> initial_ids; 
    vector<int> s_c; // Solucion Actual, se inicializa con un orden aleatorio de items
    std::srand(unsigned(std::time(0))); // Fijar la semilla, por ahora el tiempo pero esto debería poder setearse por argumentos
    // F y S para todos los restart
    vector<int> restart_s_c;
    int restart_f_s_c = 9999;

    int n = cinta_inst.get_n_objetos();
    // F y S de inicio para el 1er restart
    
    int max_iter = 30;// Controla cuantas veces, como maximo se intensifica
    int i_restarts = 0; // Controla cuantas veces se diversifica

    do{
        cout << endl;
        for(auto const& item : cinta_inst.items_to_place){
        s_c.push_back(item.get_id());
        }
        
        random_shuffle(s_c.begin(), s_c.end()); // inicializacion aleatoria de s_c
        cout << "<-------------------- Restart -------------------->" << endl;
        cout <<"* Iteracion N°"<< i_restarts << endl;
        cout << "- Punto de inicio: "; 
        display(s_c);

        Cinta tmp_cinta = cinta_inst;
        for(int const& id: s_c){
            tmp_cinta.apply_BL(cinta_inst.get_item_by_id(id));
        }
        int f_s_c = tmp_cinta.get_total_height();
        int n_iter = 30; // Controla cuantas veces, como maximo se intensifica
        bool local = false; // minimo local
        int best_height = 9999; // por ahora fijare esto asi, lo ideal sería fijarlo de acuerdo a alguna cota superior

        int best_f_s_n = f_s_c;
        vector<int> best_s_n = s_c;
        do{
            for(int i_left = 0; i_left < n-1; i_left = i_left+1){
                for(int i_right = 0; i_right < n; i_right = i_right+1){ // El vecindario esta definido por un solo swap entre dos elementos del arreglo de ids
                    vector<int> tmp_ids = s_c; // 1. Hacer una copia del arreglo original
                    Cinta tmp_cinta = cinta_inst; // Hacer una copia de la cinta
                    swap(tmp_ids[i_left], tmp_ids[i_right]); // 2. Hacer swap de las posiciones i_left con i_right, este es el movimiento
                    for(int const& item_id: tmp_ids){
                        tmp_cinta.apply_BL(tmp_cinta.get_item_by_id(item_id)); // 4. Hacer BL sobre la copia con swap y la cinta
                    }
                    int f_s_n_tmp = tmp_cinta.get_total_height(); // valor de la funcion de evaluacion para cada movimiento
                    if(f_s_n_tmp < best_f_s_n){
                        best_f_s_n = f_s_n_tmp;
                        best_s_n = tmp_ids;
                    }
                }
            }
            if(best_f_s_n <= f_s_c){ // Actualizar s_c
                f_s_c = best_f_s_n;
                s_c = best_s_n;
                n_iter++;
            }
            else{
                local = true;
            }

        }while(!local && n_iter <= max_iter); 
        cout << "- Optimo local: ";
        display(s_c);
        cout << "- Mejor valor de la funcion de evaluacion lograda: " << f_s_c << endl;

        if(f_s_c < restart_f_s_c){ // Actualizar restart_s_c
                restart_f_s_c = f_s_c;
                restart_s_c = s_c;
                cout << "--> Actualizacion del mejor optimo local obtenido (S_c)."<< endl;
            }

        s_c.clear();
        i_restarts++;
    }while(i_restarts < n_restarts);

    // Actualizar la instancia de la cinta original con el orden encontrado de articulos y aplicar BL para dejarlos emplazados
    cinta_inst.set_final_placement(restart_s_c);
    
    vector<int> final_placement = cinta_inst.get_final_placement();
    
    cout << endl;
    cout << endl;
    for(int const& id: final_placement){
        cinta_inst.apply_BL(cinta_inst.get_item_by_id(id), false);
    }

    cout << "---------- RESULTADO FINAL ----------" << endl;
    cout << "Emplazamiento final: ";
    for (auto const& id: cinta_inst.get_final_placement(false)) { 
        cout << " ("<< id << ", " << cinta_inst.get_placed_item_by_id(id).get_flipped() << ")"; 
    }
    int final_height = cinta_inst.get_total_height(true);
    int unused_area  = cinta_inst.get_max_width()*final_height - cinta_inst.calculate_used_area(true);
    cout << endl;
    cout << "Altura Final Alcanzada: " << final_height << endl;
    cout << "Espacio Inutilizado: " << unused_area << endl;
    
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
data_instancia read_instance(string file_path_buff){
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
            cout << "reading n_obj: " << instancia.n_objetos << endl;
        }
        else if (line_number == 1){
            instancia.width = atoi(line.c_str());
        }
        else{
            int item_id, item_width, item_height;

            linestream >> item_id >> item_width >> item_height;
            Item dummy_obj(item_id, item_width, item_height);
            //cout << "Pushing " << item_id << endl;
            objetos.push_back(dummy_obj);
        }
        line_number++;
    }
    infile.close();
    instancia.instance_obj = objetos;
    return instancia;
}



// Main
int main(int argc, char *argv[]) {
    auto t1 = std::chrono::high_resolution_clock::now();
    string file_name;
    data_instancia data;

    if(argc == 2){
        file_name = argv[1];
    }
    else{
        file_name = "instance.txt";
    }

    cout << "Nombre archivo de la instancia: " << file_name;
    cout << endl;
    data = read_instance(file_name);

    Cinta cinta_instancia(data.n_objetos, data.width, data.instance_obj);
    hillClimbing_MM(cinta_instancia);
    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    cout << "Tiempo de ejecucion: " << duration << "(micro s)"<< endl;

    std::regex e (".*/(.*)$");
    std::smatch sm;
    std::regex_match (file_name, sm, e);
    std::string output_filename(sm[1]);
    output_filename.append("_output.txt");
    cout << "Generando archivo de salida: " << output_filename << endl;
    ofstream outputFile;
    outputFile.open(output_filename);
    outputFile << cinta_instancia.get_total_height() << endl;
    outputFile <<  cinta_instancia.get_max_width()*cinta_instancia.get_total_height() - cinta_instancia.calculate_used_area(false) << endl;
    for (auto const& item: cinta_instancia.items_to_place) { 
        outputFile << cinta_instancia.get_placed_item_by_id(item.get_id()).get_x_LL_coord() << "\t" << cinta_instancia.get_placed_item_by_id(item.get_id()).get_y_LL_coord() << "\t" <<cinta_instancia.get_placed_item_by_id(item.get_id()).get_flipped() << endl; 
    }
    outputFile.close();
    cout << "Done!\n";


    return 0;
}

