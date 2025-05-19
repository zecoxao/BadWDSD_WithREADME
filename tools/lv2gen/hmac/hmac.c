#define BLOCK_SIZE 64

void hmac_sha1(const uint8_t *text, size_t text_len, const uint8_t *key, size_t key_len, uint8_t* digest) {
	unsigned char k_ipad[BLOCK_SIZE+1];
	unsigned char k_opad[BLOCK_SIZE+1];
	unsigned char tk[SHA1HashSize];
	if(key_len > BLOCK_SIZE) {
		SHA1Context ctx;
		SHA1Reset(&ctx);
		SHA1Input(&ctx, key, key_len);
		SHA1Result(&ctx, tk);
		key_len = SHA1HashSize;
		key = tk;
	}
	memset(k_ipad, 0, sizeof(k_ipad));
	memset(k_opad, 0, sizeof(k_opad));
	memcpy(k_ipad, key, key_len);
	memcpy(k_opad, key, key_len);
	for(int i=0; i<BLOCK_SIZE; i++) {
		k_ipad[i] ^= 0x36;
		k_opad[i] ^= 0x5c;
	}
	SHA1Context context;
	// inner
	SHA1Reset(&context);
	SHA1Input(&context, k_ipad, BLOCK_SIZE);
	SHA1Input(&context, text, text_len);
	SHA1Result(&context, digest);
	// outer
	SHA1Reset(&context);
	SHA1Input(&context, k_opad, BLOCK_SIZE);
	SHA1Input(&context, digest, SHA1HashSize);
	SHA1Result(&context, digest);
	return;
}
