switch (key_pressed)
{
    // Quit the program when 'Escape' key is pressed
    case SDLK_ESCAPE:
        quit = true;
        break;

    case SDLK_UP: // Move up
        camera_position.x += step * sin(cam_yaw);
        camera_position.z -= step * cos(cam_yaw);
        break;
    case SDLK_DOWN: // Move down
        camera_position.x -= step * sin(cam_yaw);
        camera_position.z += step * cos(cam_yaw);
        break;
    case SDLK_LEFT: // Move left
        camera_position.x -= step * cos(cam_yaw);
        camera_position.z -= step * sin(cam_yaw);
        break;
    case SDLK_RIGHT: // Move right
        camera_position.x += step * cos(cam_yaw);
        camera_position.z += step * sin(cam_yaw);
        break;
    case SDLK_u: // Move camera up
        camera_position.y += step;
        break;
    case SDLK_j: // Move camera down
        camera_position.y -= step;
        break;

    // Zoom and dezoom
    case SDLK_p: // Zoom in
        cam_dist -= zoom_step;
        if (cam_dist < 1.0) cam_dist = 1.0; // Prevent too close zoom
        break;
    case SDLK_m: // Zoom out
        cam_dist += zoom_step;
        break;

    // Rotate camera
    case SDLK_o: // Rotate left
        cam_yaw -= rotate_step;
        break;
    case SDLK_l: // Rotate right
        cam_yaw += rotate_step;
        break;
    case SDLK_k: // Rotate down
        cam_pitch += rotate_step;
        break;
    case SDLK_i: // Rotate up
        cam_pitch -= rotate_step;
        break;

    case SDLK_r:
        /*
        camera_position.x = 0;
        camera_position.y =-2.5;
        camera_position.z = 0;
        cam_dist = 19;
        cam_pitch = 0;
        cam_yaw= 1.55; */


        if (camera_position.x != 0){
            if ((camera_position.x<step)and (camera_position.x>-step)){
                camera_position.x=0;
            }
            if(camera_position.x<0){
                camera_position.x+=step;
            }
            if(camera_position.x>0){
                camera_position.x-=step;
            }

        }
        if ((camera_position.x == 0) and (camera_position.y != -2.5)){
            if ((camera_position.y<(-2.5+step))and (camera_position.y>(-2.5-step))){
                camera_position.y=-2.5;
            }
            if(camera_position.y<-2.5){
                camera_position.x+=step;
            }
            if(camera_position.y>-2.5){
                camera_position.y-=step;
            }

        }
        if ((camera_position.x == 0) and (camera_position.y == -2.5) and (camera_position.z != 0)){
            if ((camera_position.z<step)and (camera_position.z>-step)){
                camera_position.z=0;
            }
            if(camera_position.z<0){
                camera_position.z+=step;
            }
            if(camera_position.z>0){
                camera_position.z-=step;
            }
        }

        if ((camera_position.x == 0) and (camera_position.y == -2.5) and (camera_position.z == 0) and (cam_dist != 19)){
            if ((cam_dist<(zoom_step+19))and (cam_dist>(19-zoom_step))){
                cam_dist=19;
            }
            if(cam_dist<19){
                cam_dist+=zoom_step;
            }
            if(cam_dist>19){
                cam_dist-=zoom_step;
            }
        }
        if ((camera_position.x == 0) and (camera_position.y == -2.5) and (camera_position.z == 0) and (cam_dist == 19) and (cam_pitch != 0)){
            if ((cam_pitch<rotate_step)and (cam_pitch>-rotate_step)){
                cam_pitch=0;
            }
            if(cam_pitch<0){
                cam_pitch+=rotate_step;
            }
            if(cam_pitch>0){
                cam_pitch-=rotate_step;
            }
        }

        if ((camera_position.x == 0) and (camera_position.y == -2.5) and (camera_position.z == 0) and (cam_dist == 19) and (cam_pitch == 0) and(cam_yaw != 1.55) ){
            if ((cam_yaw<1.55+rotate_step)and (cam_yaw>1.55-rotate_step)){
                cam_yaw=1.55;
            }
            if(cam_pitch<1.55){
                cam_yaw+=rotate_step;
            }
            if(cam_yaw>1.55){
                //cam_yaw=cam_yaw-rotate_step;
                cam_yaw=1.55;

            }

        }

        // std::cout << "alpha:" << cam_yaw << " cam_pitch: " << cam_pitch << " cam_dist: " << cam_dist<< "x" << camera_position.x << " y" << camera_position.y << " z" << camera_position.z << std::endl;
        break;

    default:
        break;
}