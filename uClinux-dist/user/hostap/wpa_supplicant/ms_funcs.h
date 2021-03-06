#ifndef MS_FUNCS_H
#define MS_FUNCS_H

void generate_nt_response(u8 *auth_challenge, u8 *peer_challenge,
			  u8 *username, size_t username_len,
			  u8 *password, size_t password_len,
			  u8 *response);
void generate_authenticator_response(u8 *password, size_t password_len,
				     u8 *peer_challenge,
				     u8 *auth_challenge,
				     u8 *username, size_t username_len,
				     u8 *nt_response, u8 *response);
void nt_challenge_response(u8 *challenge, u8 *password, size_t password_len,
			   u8 *response);

void challenge_response(u8 *challenge, u8 *password_hash, u8 *response);
void nt_password_hash(u8 *password, size_t password_len, u8 *password_hash);
void hash_nt_password_hash(u8 *password_hash, u8 *password_hash_hash);

#endif /* MS_FUNCS_H */
