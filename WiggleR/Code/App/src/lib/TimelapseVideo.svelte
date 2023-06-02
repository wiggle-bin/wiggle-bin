<script lang="ts">
	import { start, end, showThresh, showContours } from '../store/store';

	let startTime = '';
	let endTime = '';
	let threshActive: null | boolean = false;
	let contoursActive: null | boolean = false;

	start.subscribe((time) => (startTime = time));
	end.subscribe((time) => (endTime = time));
	showThresh.subscribe((show) => (threshActive = Boolean(show)));
	showContours.subscribe((show) => (contoursActive = Boolean(show)));

	function onChangeThresh(e: { target: { active: boolean } }) {
		showThresh.update((show) => (show = e.target.active));
	}
	function onChangeContours(e: { target: { active: boolean } }) {
		showContours.update((show) => (show = e.target.active));
	}
</script>


{#key [startTime, endTime, threshActive, contoursActive]}
    <video width="100%" height="100%" controls>
        <source
            src={`http://localhost:8000/video?startTime=${startTime}&endTime=${endTime}&showThresh=${threshActive}&showContours=${contoursActive}`}
            type="video/mp4"
        />
    </video>
{/key}
<div class="video-settings">
    <kor-toggle grid-cols="2" label="Thresh" on:active-changed={onChangeThresh} active={threshActive ? true : null}></kor-toggle>
    <kor-toggle grid-cols="2" label="Contours" on:active-changed={onChangeContours}  active={contoursActive ? true : null}></kor-toggle>
</div>

<style>
	.video-settings {
		display: flex;
		gap: 10px;
	}
</style>