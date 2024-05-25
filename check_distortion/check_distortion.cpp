#include <Windows.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "resource.h"
#include <CommCtrl.h>

void print_mat4x4(const glm::mat4x4 &mat)
{
  printf("\n");
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      printf("%.3f ", mat[i][j]);
    }
    putc('\n', stdout);
  }
  printf("\n");
}

glm::mat4x4 trimetric_project_matrix(float p, float q, float r)
{
  glm::mat4x4 projection = glm::mat4x4(
    glm::vec4(1.f, 0.f, 0.f, 0.f),
    glm::vec4(0.f, 1.f, 0.f, 0.f),
    glm::vec4(0.f, 0.f, 0.f, r),
    glm::vec4(0.f, 0.f, 0.f, 1.f)
  );

  glm::mat4x4 rx = glm::rotate(glm::mat4x4(1.f), glm::radians(p), glm::vec3(1.f, 0.f, 0.f));
  glm::mat4x4 ry = glm::rotate(glm::mat4x4(1.f), glm::radians(q/* + 90.f*/), glm::vec3(0.f, 1.f, 0.f));
  glm::mat4x4 complex_mat = projection * ry * rx;
  print_mat4x4(complex_mat);
  return complex_mat;
}

void test()
{
  glm::mat4x4 projection = trimetric_project_matrix(0.f, 0.f, 1.f);
  

  glm::vec4 orts[] = {
    glm::vec4(1.f, 0.f, 0.f, 1.f), //x
    glm::vec4(0.f, 1.f, 0.f, 1.f), //y
    glm::vec4(0.f, 0.f, 1.f, 1.f)  //z
  };

  const size_t num_verts = sizeof(orts) / sizeof(orts[0]);
  glm::vec4 projected_orts[num_verts];
  printf("--------- orts -------\n");
  for (size_t i = 0; i < num_verts; i++) {
    glm::vec4 &coord = orts[i];
    printf("%.3f %.3f %.3f %.3f\n", coord.x, coord.y, coord.z, coord.w);
  }
  printf("----------------------\n");

  printf("--------- protected -------\n");
  for (size_t i = 0; i < num_verts; i++) {
    glm::vec4 &coord = projected_orts[i] = projection * orts[i];
    printf("%.3f %.3f %.3f %.3f\n", coord.x, coord.y, coord.z, coord.w);
  }
  printf("----------------------\n");

  float pxlen = glm::length(glm::vec2(projected_orts[0]));
  float pylen = glm::length(glm::vec2(projected_orts[1]));
  float pzlen = glm::length(glm::vec2(projected_orts[2]));
  printf(
    "xd: %.3f\n"
    "yd: %.3f\n"
    "zd: %.3f\n",
    pxlen / 1,
    pylen / 1,
    pzlen / 1
  );
}

float edit_get_float(HWND h_control)
{
  char buf[128];
  GetWindowTextA(h_control, buf, sizeof(buf));
  return (float)atof(buf);
}

template<const size_t dimi, const size_t dimj>
class matrix_control
{
  HWND h_controls[dimi][dimj];

public:
  matrix_control() {}
  ~matrix_control() {}

  bool resolve_controls(HWND h_parent, const int *p_controls_ids) {
    bool b_success = true;
    for (size_t i = 0; i < dimi; i++) {
      for (size_t j = 0; j < dimj; j++) {
        h_controls[i][j] = GetDlgItem(h_parent, p_controls_ids[i * dimi + j]);
        b_success &= (int)(h_controls[i][j] != NULL);
      }
    }
    return b_success;
  }

  float get_value(const size_t i, const size_t j) {
    char strval[64];
    GetWindowTextA(h_controls[i][j], strval, sizeof(strval));
    return (float)atof(strval);
  }

  void set_value(const size_t i, const size_t j, float val) {
    char strval[64];
    snprintf(strval, sizeof(strval), "%.3f", val);
    SetWindowTextA(h_controls[i][j], strval);
  }

  void set_value(const glm::mat4x4 &mat) {
    for (size_t i = 0; i < dimi; i++) {
      for (size_t j = 0; j < dimj; j++) {
        set_value(i, j, mat[i][j]);
      }
    }
  }

  glm::mat4x4 get_value() {
    glm::mat4x4 mat;
    for (size_t i = 0; i < dimi; i++) {
      for (size_t j = 0; j < dimj; j++) {
        mat[i][j] = get_value(i, j);
      }
    }
    return mat;
  }
};

template<const size_t size>
class vec4_control
{
  HWND h_controls[size];
public:
  vec4_control() {}
  ~vec4_control() {}

  bool resolve_controls(HWND h_parent, const int *p_controls_ids) {
    bool b_success = true;
    for (size_t i = 0; i < size; i++) {
        h_controls[i] = GetDlgItem(h_parent, p_controls_ids[i]);
        b_success &= (int)(h_controls[i] != NULL);
    }
    return b_success;
  }

  float get_value(const size_t i) {
    char strval[64];
    GetWindowTextA(h_controls[i], strval, sizeof(strval));
    return (float)atof(strval);
  }

  void set_value(const size_t i, float val) {
    char strval[64];
    snprintf(strval, sizeof(strval), "%.3f", val);
    SetWindowTextA(h_controls[i], strval);
  }

  void get_value(glm::vec2 &dst) { dst = glm::vec2(get_value(0), get_value(1)); }
  void get_value(glm::vec3 &dst) { dst = glm::vec3(get_value(0), get_value(1), get_value(2)); }
  void get_value(glm::vec4 &dst) { dst = glm::vec4(get_value(0), get_value(1), get_value(2), get_value(3)); }

  void set_value(const glm::vec2 v) {
    set_value(0, v.x);
    set_value(1, v.y);
  }

  void set_value(const glm::vec3 v) {
    set_value(0, v.x);
    set_value(1, v.y);
    set_value(2, v.z);
  }

  void set_value(const glm::vec4 v) {
    set_value(0, v.x);
    set_value(1, v.y);
    set_value(2, v.z);
    set_value(3, v.w);
  }
};

HWND h_scroll_xangle;
HWND h_scroll_yangle;
HWND h_anglex_text;
HWND h_angley_text;
HWND h_edit_anglex;
HWND h_edit_angley;
matrix_control<4, 4> mat_proj;
matrix_control<4, 4> mat_rotx;
matrix_control<4, 4> mat_roty;
matrix_control<4, 4> mat_complex;
vec4_control<4> ortx, orty, ortz, orttx, ortty, orttz;
vec4_control<3> length_transformed_orts;
vec4_control<3> length_orts;
vec4_control<3> dist_coeffs;

glm::mat4x4 proj, rotx, roty, complex_mat;
float anglex = 0.f, angley = 0.f;

void static_set_text(HWND h_control, const char *p_format, ...)
{
  char buf[1024];
  va_list argptr;
  va_start(argptr, p_format);
  vsnprintf_s(buf, sizeof(buf), p_format, argptr);
  va_end(argptr);
  SetWindowTextA(h_control, buf);
}

const int proj_mat_controls[] = {
  IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4,
  IDC_EDIT5, IDC_EDIT6, IDC_EDIT7, IDC_EDIT8,
  IDC_EDIT9, IDC_EDIT10, IDC_EDIT11, IDC_EDIT12,
  IDC_EDIT13, IDC_EDIT14, IDC_EDIT15, IDC_EDIT16
};

const int mat_rotx_controls[] = {
  IDC_EDIT17, IDC_EDIT18, IDC_EDIT19, IDC_EDIT20,
  IDC_EDIT21, IDC_EDIT22, IDC_EDIT23, IDC_EDIT24,
  IDC_EDIT25, IDC_EDIT26, IDC_EDIT27, IDC_EDIT28,
  IDC_EDIT29, IDC_EDIT30, IDC_EDIT31, IDC_EDIT32
};

const int mat_roty_controls[] = {
  IDC_EDIT33, IDC_EDIT34, IDC_EDIT35, IDC_EDIT36,
  IDC_EDIT37, IDC_EDIT38, IDC_EDIT39, IDC_EDIT40,
  IDC_EDIT41, IDC_EDIT42, IDC_EDIT43, IDC_EDIT44,
  IDC_EDIT45, IDC_EDIT46, IDC_EDIT47, IDC_EDIT48
};

const int mat_complex_controls[] = {
  IDC_EDIT49, IDC_EDIT50, IDC_EDIT51, IDC_EDIT52,
  IDC_EDIT53, IDC_EDIT54, IDC_EDIT55, IDC_EDIT56,
  IDC_EDIT57, IDC_EDIT58, IDC_EDIT59, IDC_EDIT60,
  IDC_EDIT61, IDC_EDIT62, IDC_EDIT63, IDC_EDIT64
};

const int ortx_ids[] = { IDC_EDIT65, IDC_EDIT66, IDC_EDIT67, IDC_EDIT68 };
const int orty_ids[] = { IDC_EDIT69, IDC_EDIT70, IDC_EDIT71, IDC_EDIT72 };
const int ortz_ids[] = { IDC_EDIT73, IDC_EDIT74, IDC_EDIT75, IDC_EDIT76 };
const int ortx_transformed_ids[] = { IDC_EDIT77, IDC_EDIT78, IDC_EDIT79, IDC_EDIT80 };
const int orty_transformed_ids[] = { IDC_EDIT81, IDC_EDIT82, IDC_EDIT83, IDC_EDIT84 };
const int ortz_transformed_ids[] = { IDC_EDIT85, IDC_EDIT86, IDC_EDIT87, IDC_EDIT88 };
const int transformed_orts_length_ids[] = { IDC_EDIT89, IDC_EDIT90, IDC_EDIT91 };
const int orts_length_ids[] = { IDC_EDIT92, IDC_EDIT93, IDC_EDIT94 };
const int coeffs_ids[] = { IDC_EDIT95, IDC_EDIT96, IDC_EDIT97 };

#define EPSILON (0.00001f)

#define ZERO_CHECK(x) if(x < EPSILON) x = 1.f;

//TODO: добавить проверку на то что инпут пуст, чтобы брать значения из трекбара
bool is_edit_empty(HWND h_control)
{
  char sym[64];
  GetWindowTextA(h_control, sym, sizeof(sym));
  return !sym[0];
}

void compute()
{
  glm::vec4 src_orts[3], dst_orts[3];

  if (!is_edit_empty(h_edit_anglex))
    anglex = edit_get_float(h_edit_anglex);
  if (!is_edit_empty(h_edit_angley))
    angley = edit_get_float(h_edit_angley);

  static_set_text(h_anglex_text, "angle X %.3f deg (%.3f rad)", anglex, glm::radians(anglex));
  static_set_text(h_angley_text, "angle Y %.3f deg (%.3f rad)", angley, glm::radians(angley));

  rotx = glm::rotate(glm::mat4x4(1.f), anglex, glm::vec3(1.f, 0.f, 0.f));
  roty = glm::rotate(glm::mat4x4(1.f), angley, glm::vec3(0.f, 1.f, 0.f));
  mat_rotx.set_value(rotx);
  mat_roty.set_value(roty);

  proj = mat_proj.get_value();
  rotx = mat_rotx.get_value();
  roty = mat_roty.get_value();
  complex_mat = proj * roty * rotx;
  print_mat4x4(complex_mat);
  mat_complex.set_value(complex_mat);

  /* get orts */
  ortx.get_value(src_orts[0]);
  orty.get_value(src_orts[1]);
  ortz.get_value(src_orts[2]);

  /* transform */
  for (size_t i = 0; i < 3; i++)
    dst_orts[i] = complex_mat * src_orts[i];

  /* print not normalized */
  for (size_t i = 0; i < 3; i++) {
    glm::vec4 &rort = dst_orts[i];
    printf("nn %.3f %.3f %.3f %.3f\n", rort.x, rort.y, rort.z, rort.w);
  }
  
  /* normalize orts if w != 0 */
  for (size_t i = 0; i < 3; i++) {
    glm::vec4 &rort = dst_orts[i];
    if (rort.w > EPSILON) {
      rort /= rort.w;
    }
    printf("%zd transformed ort %.3f %.3f %.3f %.3f\n", i, rort.x, rort.y, rort.z, rort.w);
  }

  /* set transformed orts */
  orttx.set_value(dst_orts[0]);
  ortty.set_value(dst_orts[1]);
  orttz.set_value(dst_orts[2]);

  /* transformed orts length */
  glm::vec3 transformed_orts_len;
  transformed_orts_len.x = glm::length(glm::vec2(dst_orts[0]));
  transformed_orts_len.y = glm::length(glm::vec2(dst_orts[1]));
  transformed_orts_len.z = glm::length(glm::vec2(dst_orts[2]));
  length_transformed_orts.set_value(transformed_orts_len);

  /* source orts length */
  glm::vec3 orts_len;
  orts_len.x = glm::length(glm::vec3(src_orts[0]));
  orts_len.y = glm::length(glm::vec3(src_orts[1]));
  orts_len.z = glm::length(glm::vec3(src_orts[2]));
  ZERO_CHECK(orts_len.x)
  ZERO_CHECK(orts_len.y)
  ZERO_CHECK(orts_len.z)
  length_orts.set_value(orts_len);

  /* distortion factors */
  glm::vec3 distortion = transformed_orts_len / orts_len;
  dist_coeffs.set_value(distortion);
}

void init()
{
  proj = glm::mat4x4(
    glm::vec4(1.f, 0.f, 0.f, 0.f),
    glm::vec4(0.f, 1.f, 0.f, 0.f),
    glm::vec4(0.f, 0.f, 0.f, 0.1f),
    glm::vec4(0.f, 0.f, 0.f, 1.f)
  );
  rotx = glm::rotate(glm::mat4x4(1.f), anglex, glm::vec3(1.f, 0.f, 0.f));
  roty = glm::rotate(glm::mat4x4(1.f), angley, glm::vec3(0.f, 1.f, 0.f));
  mat_proj.set_value(proj);
  mat_rotx.set_value(rotx);
  mat_roty.set_value(roty);
  ortx.set_value(glm::vec4(1.f, 0.f, 0.f, 1.f));
  orty.set_value(glm::vec4(0.f, 1.f, 0.f, 1.f));
  ortz.set_value(glm::vec4(0.f, 0.f, 1.f, 1.f));
  compute();
}

LPARAM CALLBACK proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
  {
  case WM_COMMAND: {
    int id = LOWORD(wparam);
    if (id == IDOK) {
      printf("OK\n");
      compute();
    }
    break;
  }

  case WM_CLOSE:
    PostQuitMessage(0);
    break;

  case WM_HSCROLL:
    anglex = (float)SendMessageA(h_scroll_xangle, TBM_GETPOS, (WPARAM)0, (LPARAM)0);
    //printf("anglex: %f\n", anglex);
    angley = (float)SendMessageA(h_scroll_yangle, TBM_GETPOS, (WPARAM)0, (LPARAM)0);
    //printf("angley: %f\n", angley);
    compute();
    break;

  default:
    return DefWindowProcA(hwnd, msg, wparam, lparam);
  }
  return 0;
}

void init_interface()
{
  HWND h_dialog = CreateDialogA(GetModuleHandleW(0), MAKEINTRESOURCEA(IDD_DIALOG1), HWND_DESKTOP, (DLGPROC)&proc);
  mat_proj.resolve_controls(h_dialog, proj_mat_controls);
  mat_rotx.resolve_controls(h_dialog, mat_rotx_controls);
  mat_roty.resolve_controls(h_dialog, mat_roty_controls);
  mat_complex.resolve_controls(h_dialog, mat_complex_controls);
  ortx.resolve_controls(h_dialog, ortx_ids);
  orty.resolve_controls(h_dialog, orty_ids);
  ortz.resolve_controls(h_dialog, ortz_ids);
  orttx.resolve_controls(h_dialog, ortx_transformed_ids);
  ortty.resolve_controls(h_dialog, orty_transformed_ids);
  orttz.resolve_controls(h_dialog, ortz_transformed_ids);
  length_transformed_orts.resolve_controls(h_dialog, transformed_orts_length_ids);
  length_orts.resolve_controls(h_dialog, orts_length_ids);
  dist_coeffs.resolve_controls(h_dialog, coeffs_ids);
  h_scroll_xangle = GetDlgItem(h_dialog, IDC_SLIDER1);
  h_scroll_yangle = GetDlgItem(h_dialog, IDC_SLIDER2);
  SendMessageA(h_scroll_xangle, TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)-90);
  SendMessageA(h_scroll_xangle, TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)90);
  SendMessageA(h_scroll_xangle, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
  SendMessageA(h_scroll_yangle, TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)-90);
  SendMessageA(h_scroll_yangle, TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)90);
  SendMessageA(h_scroll_yangle, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
  h_anglex_text = GetDlgItem(h_dialog, IDC_STATIC39);
  h_angley_text = GetDlgItem(h_dialog, IDC_STATIC40);
  h_edit_anglex = GetDlgItem(h_dialog, IDC_EDIT98);
  h_edit_angley = GetDlgItem(h_dialog, IDC_EDIT99);
  init();
  ShowWindow(h_dialog, SW_SHOW);
  UpdateWindow(h_dialog);

  MSG msg;
  while (GetMessageA(&msg, 0, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

int main()
{
  const float p = -45.f;
  const float q = 45.f;
  const float r = 0.5f;
  glm::mat4x4 complex_mat = trimetric_project_matrix(p, q, r);
#if 0
  /* print source verts */
  printf("----- source verts -----\n");
  for (size_t i = 0; i < verts_count; i++) {
    glm::vec3 &coord = verts[i];
    printf("coord %.3f %.3f %.3f\n", coord.x, coord.y, coord.z);
  }
  printf("------------------------\n");

  /* project */
  static glm::vec3 projected[verts_count];
  printf("----- protected verts -----\n");
  for (size_t i = 0; i < verts_count; i++) {
    glm::vec4 gcoord = complex_mat * glm::vec4(verts[i], 1.f);
    printf("coord: %.3f %.3f %.3f %.3f\n", gcoord.x, gcoord.y, gcoord.z, gcoord.w);
    projected[i] = gcoord;
  }
  printf("------------------------\n");

  float srcl = glm::length(glm::vec2(verts[0]));
  printf("src length: %.3f\n", srcl);

  float protectedl = glm::length(glm::vec2(projected[0]));
  printf("protected length: %.3f\n", protectedl);

  if (protectedl < 0.000001f)
    protectedl = 1.f;

  float coeff = protectedl / srcl;
  printf(
    "srcl = %f\n"
    "srcd = %f\n"
    "coeff = %f\n"
    "coeff_mat p = %f\n"
    "coeff_mat q = %f\n"
    "coeff_mat r = %f\n",
    srcl,
    protectedl,
    coeff,
    complex_mat[0][3],
    complex_mat[1][3],
    complex_mat[2][3]
  );
#endif

  //glm::vec4 coord1(1.f, 0.f, 0.f, 1.f);
  //glm::vec4 projected = complex_mat * coord1;

  //float src_len = glm::length(glm::vec3(coord1));
  //float projected_len = glm::length(glm::vec2(projected));
  //float dist1 = projected_len / src_len;
  //printf(
  //  "srcLen: %.3f\n"
  //  "projectedLen: %.3f\n"
  //  "distortion1: %.3f\n",
  //  src_len, projected_len, dist1
  //);
  //test();
  //init_interface();

  glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), glm::vec3(0.f, 0.f, 10.f));
  float *p_value = (float *)glm::value_ptr(mat);
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      printf("%f ", p_value[i * 4 + j]);
    }
    printf("\n");
  }


  return 0;
}