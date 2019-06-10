#ifndef _TAPPCTRL_JSON_H_
#define _TAPPCTRL_JSON_H_

#ifdef __cplusplus
extern "C" {

class TappJsonDocument;
class TappJsonValue;

#else

typedef struct tagTappJsonDocument TappJsonDocument;
typedef struct tagTappJsonValue    TappJsonValue;

#endif

/* ******************************************************
 *
 * !! Attention
 *
 * This feature only supports for c++ (g++).
 * DO NOT use these functions for gcc.
 * You can link only by g++
 *
 * ****************************************************** */

int tapp_json_alloc( TappJsonDocument** doc );
int tapp_json_free( TappJsonDocument* doc );

int tapp_json_parse( TappJsonDocument* doc, const char* json );
int tapp_json_to_string( TappJsonDocument* doc, char* buffer, int buffer_length );

/*  How to write the key of tapp json
 *
 *  e.g. "key", "key1:key2:key3", "key1:[3]:key3", "key1:key2:[0]"
 *  These function NOT need to verify or add special of their key.
 */
int tapp_json_has_member( TappJsonDocument* doc, const char* key );
int tapp_json_remove_member( TappJsonDocument* doc, const char* key );
int tapp_json_remove_all_members( TappJsonDocument* doc );

int tapp_json_set_int( TappJsonDocument* doc, const char* key, int value );
int tapp_json_set_int64_t( TappJsonDocument* doc, const char* key, int64_t value );
int tapp_json_set_string( TappJsonDocument* doc, const char* key, const char* value );

int tapp_json_get_int( TappJsonDocument* doc, const char* key, int* value );
int tapp_json_get_int64_t( TappJsonDocument* doc, const char* key, int64_t* value );
int tapp_json_get_string( TappJsonDocument* doc, const char* key, const char** value );


#ifdef __cplusplus
}
#endif

#endif /* _TAPPCTRL_JSON_H_ */

