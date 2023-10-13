#include "core.h"

struct texture{
  int id;
  int width;
  int height;
  int format;
  int mipmap;
};

#define TEXTURE_LIST_SIZE 512

int main(){
  printf("Texture type: Size:%llu Total:%llu\n", sizeof(struct texture), TEXTURE_LIST_SIZE * sizeof(struct texture));

  struct handle_list_f  texture_list = init_handle_list_f(sizeof(struct texture), TEXTURE_LIST_SIZE);

  struct texture new_texture_safe_objet;
  new_texture_safe_objet.id = -1;
  new_texture_safe_objet.width = 128;
  new_texture_safe_objet.height = 128;
  new_texture_safe_objet.format = 32;
  new_texture_safe_objet.mipmap = 1;

  set_safe_object_handle_list_f(&texture_list, &new_texture_safe_objet);

  struct texture new_texture_player;
  new_texture_player.id = 00;
  new_texture_player.width = 1280;
  new_texture_player.height = 768;
  new_texture_player.format = 32;
  new_texture_player.mipmap = 1;

  struct texture new_texture_level;
  new_texture_level.id = 01;
  new_texture_level.width = 1920;
  new_texture_level.height = 1080;
  new_texture_level.format = 32;
  new_texture_level.mipmap = 1;

  struct texture new_texture_enemy;
  new_texture_enemy.id = 02;
  new_texture_enemy.width = 1600;
  new_texture_enemy.height = 900;
  new_texture_enemy.format = 32;
  new_texture_enemy.mipmap = 1;

  struct texture new_texture_ship;
  new_texture_ship.id = 03;
  new_texture_ship.width = 800;
  new_texture_ship.height = 600;
  new_texture_ship.format = 32;
  new_texture_ship.mipmap = 1;


  struct handle_f player_sprite = add_handle_list_f(&texture_list, &new_texture_player);
  //remove_handle_list_f(&texture_list, player_sprite);
  struct handle_f level_sprite = add_handle_list_f(&texture_list, &new_texture_level);
  struct handle_f enemy_sprite = add_handle_list_f(&texture_list, &new_texture_enemy);
  struct handle_f ship_sprite = add_handle_list_f(&texture_list, &new_texture_ship);

  //reset_handle_list_f(&texture_list, TRUE, 10);
  //remove_handle_list_f(&texture_list, player_sprite);
  //free_handle_list_f(&texture_list);
  if(is_valid_handle_f(&texture_list, player_sprite)){
    printf("\nplayer_sprite Is Valid\n");
  }else{
    printf("\nplayer_sprite Not Valid\n");
  }

  printf("\n\n");
  struct texture *txd_player = (struct texture*)lookup_f(&texture_list, player_sprite);
  printf("Texture Info Player slot:00 => Id:%d, Width:%d, Height:%d, Format:%d, Mipmap:%d\n", txd_player->id, txd_player->width, txd_player->height,  txd_player->format, txd_player->mipmap);

  struct texture *txd_level = (struct texture*)lookup_f(&texture_list, level_sprite);
  printf("Texture Info Level  slot:01 => Id:%d, Width:%d, Height:%d, Format:%d, Mipmap:%d\n", txd_level->id, txd_level->width, txd_level->height,  txd_level->format, txd_level->mipmap);

  struct texture *txd_enemy = (struct texture*)lookup_f(&texture_list, enemy_sprite);
  printf("Texture Info Enemy  slot:02 => Id:%d, Width:%d, Height:%d, Format:%d, Mipmap:%d\n", txd_enemy->id, txd_enemy->width, txd_enemy->height, txd_enemy->format, txd_enemy->mipmap);

  struct texture *txd_ship = (struct texture*)lookup_f(&texture_list, ship_sprite);
  printf("Texture Info Ship   slot:03 => Id:%d, Width:%d, Height:%d, Format:%d, Mipmap:%d\n", txd_ship->id, txd_ship->width, txd_ship->height, txd_ship->format, txd_ship->mipmap);
  printf("\n\n");


  printf("Handle List Info    => Slots Count:%d, Free Slots Count:%d, Start Free Slot:%d\n", texture_list.slot_count, texture_list.free_slot_count, texture_list.start_free_slot);

  return 0;
}