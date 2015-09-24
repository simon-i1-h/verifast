#ifndef AES_H
#define AES_H

#include "macro_defines.h"
#include "net.h"

#define AES_ENCRYPT     1
#define AES_DECRYPT     0

struct aes_context
{
  char content[AES_CONTEXT_SIZE];
};
typedef struct aes_context aes_context;

/*@

predicate aes_context(aes_context *context) =
  chars((void*) context, AES_CONTEXT_SIZE, _) &*&
  struct_aes_context_padding(context)
;

predicate aes_context_initialized(aes_context *context,
                                  int principal, int count);

@*/

int aes_setkey_enc(aes_context *ctx, const char *key, unsigned int keysize);
  /*@ requires [?f]cryptogram(key, ?size_key, ?cs_key, ?cg_key) &*& 
                 keysize == size_key * 8 &*&
                 cg_key == cg_symmetric_key(?p, ?c) &*&
               aes_context(ctx) &*&
               (keysize == 128 || keysize == 192 || keysize == 256); @*/
  /*@ ensures  [f]cryptogram(key, size_key, cs_key, cg_key) &*& 
               result == 0 ?
                 aes_context_initialized(ctx, p, c)
               :
                 aes_context(ctx); @*/

void aes_free(aes_context *ctx);
  //@ requires aes_context_initialized(ctx, _, _);
  //@ ensures  aes_context(ctx);

int aes_crypt_cfb128(aes_context *ctx, int mode, size_t length, 
                                size_t *iv_off, char *iv, 
                                const char *input, char *output);
  /*@ requires aes_context_initialized(ctx, ?p, ?c) &*&
               (mode == AES_ENCRYPT || mode == AES_DECRYPT) &*&
               chars(iv, 16, ?cs_iv) &*& u_integer(iv_off, ?iv_off_val) &*&
               iv_off_val >= 0 &*& iv_off_val < 16 &*&
               [?f]optional_crypto_chars(?cc, input, length, ?cs_input) &*&
                 length >= MIN_ENC_SIZE &*&
               chars(output, length, _); @*/
  /*@ ensures aes_context_initialized(ctx, p, c) &*&
              chars(iv, 16, _) &*& u_integer(iv_off, _) &*&
              [f]optional_crypto_chars(cc, input, length, cs_input) &*&
              result == 0 ?
                mode == AES_ENCRYPT ?
                (
                  cryptogram(output, length, _, ?cg) &*&
                  cg == cg_encrypted(p, c, cs_input,
                                     append(chars_of_int(iv_off_val), cs_iv))
                ) :
                (
                  collision_in_run ?
                    chars(output, length, ?cs_output)
                  :
                    crypto_chars(output, length, ?cs_output) &*&
                    cs_input == chars_for_cg(cg_encrypted(p, c, cs_output, 
                                      append(chars_of_int(iv_off_val), cs_iv)))
                )
              :
                chars(output, length, _); @*/

#endif